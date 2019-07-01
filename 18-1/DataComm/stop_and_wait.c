/*
 *  201803 SKKU DATACOMM PROJECT by 이태진 교수님
 *
 *  1. Input the # of packets to send / simulation time(T) / Frame loss and corruption probability
 *  2. Verify the program if it works correctly.
 *  3. Measure the # of correctly received bytes(B). And compute the throughput(B*8/T) for each simulation. Run many times to get the avery throughput.
 *  4. Measure the frame transmission delay between the start of frame transmission and the end of successful frame reception for each of a new frame under the frame loss/corruption probability. You can add them and divide it by the number of successfully received frames during T. You may run many simulations to get the average delay.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#define  TIMER_INTERRUPT 0
#define  SENDING         1
#define  RECEIVING       2
#define  NOUPDATE        0
#define  UPDATE          1
#define  ARRIVE_LINK     2
#define  A               0
#define  B               1
#define  pkt_size        2000       //하나의 패킷 사이즈(BYTE)
#define  retran_limit    4
#define  M               1000000
#define  K               1000

typedef struct dgm {
    char data[pkt_size];
}DGM;
typedef struct frm {
    int seqnum;
    int acknum;
//    int checksum;
    char payload[pkt_size];
}FRM;

FRM *B_current_frm;
FRM *A_current_frm;
FRM *copied_frm;
DGM *dgm_received=NULL;


typedef struct event{
    float evtime;          // event time
    int evtype;            // event type code
    int eventity;          // entity where event occurs
    FRM *frmptr;           // ptr to frame (if any) associated with this event
    struct event *prev;   // previous event pointer
    struct event *next;   // next event pointer
}EVENT;
EVENT *evlist = NULL;     // the event list

int   TRACE = 1;           // for my debugging
int   nFraming = 0;        // number of datagrams from 3 to 2 so far
int   nFramingMax = 0;     // number of max datagrams to generate, then stop
float ttime = 0.000;      // in millisec
float lossprob;            // probability that a frame is dropped
float corruptprob;         // probability that one bit in frame is flipped
float lambda=0.05;        // arrival rate of datagrams from layer 3
float RTT=11;             // RTT
int   ntolayer1;           // number of frames sent into layer 1
int   nlost;               // number of frames lost in media
int   ncorrupt;            // number of frames corrupted by media
int continuous_retran;     // 연속으로 다시 전송한 횟수
int SIM_TIME;            // 시뮬레이션 시간

int num_just_sent;         // 전송된 frame 수
int num_correct_received;  // 올바른 frame을 받은 횟수
int BYTES_TO_BE_SENT;
int TIMEOUT_MSEC;
int CURRENT_SEQ;
int CURRENT_ACK;
float TOTAL_TRANS_DELAY;
float SENT_TIMESTAMP;
float RECEIVED_TIMESTAMP;

float jimsrand() {
    return (float)rand()/RAND_MAX;
}

void insertevent(EVENT *p) {
    EVENT *q=NULL, *qold=NULL;
    
    q = evlist;      // q points to header of list in which p struct inserted
    if (q==NULL) {   // list is empty
        evlist=p;
        p->next=NULL;
        p->prev=NULL;
    }
    else {
        // 시간이 같은 경우, 나중에 들어온 이벤트가 뒤로 간다.
        for (qold = q; q !=NULL && p->evtime >= q->evtime; q=q->next)
            qold=q;
        if (q==NULL) {   // end of list
            qold->next = p;
            p->prev = qold;
            p->next = NULL;
        }
        else if (q==evlist) { // front of list
            p->next=evlist;
            p->prev=NULL;
            p->next->prev=p;
            evlist = p;
        }
        else {     // middle of list
            p->next=q;
            p->prev=q->prev;
            q->prev->next=p;
            q->prev=p;
        }
    }
}
void deleteEvent(EVENT *p){
    if(p==NULL){
        printf("WARNING NO EVENT TO DELETE\n");
        return;
    }
    // ROOT 이벤트
    if(p == evlist){
        // evlist 길이 >= 2
        if(evlist->next != NULL){
            evlist = evlist->next;
            evlist->prev= NULL;
            p->prev = NULL;
            p->next = NULL;
            free(p);
            p=NULL;
        }
            // evlist 길이 == 1
        else{
            evlist=NULL;
            p->prev = NULL;
            p->next = NULL;
            free(p);
            p=NULL;
        }
        
    }
        // 마지막 이벤트
    else if(p->next == NULL){
        p->prev->next = NULL;
        p->prev=NULL;
        p->next=NULL;
        free(p);
        p=NULL;
    }
        // 중간 이벤트
    else{
        p->prev->next = p->next;
        p->next->prev = p->prev;
        p->prev=NULL;
        p->next=NULL;
        free(p);
        p=NULL;
    }
}
EVENT* generate_event(int toWhom, int where, int timeset) {
    double x;
    EVENT *evptr=NULL;
    
    if (TRACE>2)
        printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");
    
    if(timeset==UPDATE)
        x = 1 + 9*jimsrand();  // x is uniform on [0,5.5]
    else if(timeset==ARRIVE_LINK)
        x = 0.01 + 0.08*jimsrand();
    else
        x = 0;                // Processsing delay is ignored.
    
    evptr = (EVENT *)malloc(sizeof(EVENT));
    evptr->evtime =  ttime + x;
    evptr->eventity = toWhom;
    evptr->evtype = where;
    evptr->eventity = toWhom;
    insertevent(evptr);
    
    return evptr;
}

void printevlist() {
    EVENT *q=NULL;
    printf("--------------\nEvent List Follows:\n");
    for(q = evlist; q!=NULL; q=q->next) {
        printf("Event time: %f, type: %d entity: %d\n",q->evtime,q->evtype,q->eventity);
    }
    printf("--------------\n");
}

void startTimer(FRM* frm) {
    
    EVENT *q=NULL, *evptr=NULL;
    
    if (TRACE>2)
        printf("          START TIMER: starting timer at %f\n",ttime);
    
    // be nice: check to see if timer is already started, if so, then  warn
    for (q=evlist; q!=NULL ; q = q->next){
        if (q->evtype==TIMER_INTERRUPT) {
            printf("WARNING TIMER_START_AGAIN - The timer is already started\n");
            return;
        }
    }
    
    // create future event for when timer goes off
    evptr = (EVENT*)malloc(sizeof(EVENT));
    evptr->evtime =  ttime + TIMEOUT_MSEC;   // 타이머가 울리는 미래의 시간
    evptr->evtype =  TIMER_INTERRUPT;
    evptr->eventity = A;
    evptr->frmptr = frm;
    insertevent(evptr);
}
void stopTimer() {  // A or B is trying to stop timer
    EVENT *q=NULL;
    
    if (TRACE>2)
        printf("          STOP TIMER: stopping timer at %f\n",ttime);
    for (q=evlist; q!=NULL ; q = q->next){
        if(q->evtype==TIMER_INTERRUPT){
            // remove this event
            if (q->next==NULL && q->prev==NULL)
                evlist=NULL;  // remove first and only event on list
            else if (q->next==NULL) // end of list - there is one in front
                q->prev->next = NULL;
            else if (q==evlist) {   // front of list - there must be event after
                q->next->prev=NULL;
                evlist = q->next;
            }
            else {     // middle of list
                q->next->prev = q->prev;
                q->prev->next =  q->next;
            }
            free(q);
            return;
        }
    }
}

FRM* framing_dgm() {
    int i, j;
    FRM* frm2give=NULL;
    DGM* dgm=NULL;

//    if (nFraming==nFramingMax){
//        // STOP
//    }
    
    dgm = (DGM*)malloc(sizeof(DGM));
    j = nFraming % 26;
    for (i=0; i<pkt_size; i++)
        dgm->data[i] = 97 + j;
    nFraming++;
    
    frm2give = (FRM*)malloc(sizeof(FRM));
    for(i=0; i<pkt_size; i++){
        frm2give->payload[i] = dgm->data[i];
    }
    frm2give->seqnum = CURRENT_SEQ;
    return frm2give;
}

void A_init() {
    BYTES_TO_BE_SENT = 2000;
    TIMEOUT_MSEC = 15;
    CURRENT_SEQ=0;
}
void B_init() {
    CURRENT_ACK=0;
}
void init() {
    int i;
    int MODE;
    float sum, avg;
    
    printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n");
    //printf("MODE: 1(GET INPUT), 2(AUTO: 10,000 packet & 3,600 sec): ");
    //scanf("%d",&MODE);
    //printf("\n");
    MODE = 1;
    if(MODE==1){
        //printf("PACKETS TO SEND     : ");
        //scanf("%d",&nFramingMax);
        nFramingMax = 10000;
        
        printf("SIMULATION TIME(sec): ");
        scanf("%d",&SIM_TIME);
        SIM_TIME*=1000;
        
    }else if(MODE==2){
        nFramingMax = 10000;  // 10K 패킷
        SIM_TIME = 3600;      // 한 시간
        SIM_TIME*=1000;
    }else{
        printf("WRONG INPUT: TERMINATES THE PROGRAM\n");
        exit(0);
    }
    
    //printf("LOSS/CORRUPTION PROBABILITY: ");
    //scanf("%f",&lossprob);
    lossprob = 0.1;
    if(lossprob > 1 || lossprob<0){
        printf("PROBABILITIY IS OUT OF BOUNDARY[0.0~1.0].\nTERMINATES THE PROGRAM.\n\n");
        exit(0);
    }
    
    corruptprob = lossprob;

//    printf("Frame Corruption Probability [0.0 ~ 1.0] : ");
//    scanf("%f",&corruptprob);
//    corruptprob = 0.1;
    
    srand((unsigned)time(NULL));         // init random number generator
    sum = 0.0;                  // test random number generator for students
    for (i=0; i<1000; i++){
        sum=sum+jimsrand();    // jimsrand() should be uniform in [0,1]
    }
    
    avg = sum/1000;
    if (avg < 0.25 || avg > 0.75) {
        printf("It is likely that random number generation on your machine\n" );
        printf("is different from what this emulator expects.  Please take\n");
        printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
        exit(0);  // 프로그램 완전 종료
    }
    
    printf("-----------------------------------------------------------------------\n");
    
    ntolayer1 = 0;
    nlost = 0;
    ncorrupt = 0;
    
    ttime=0.0;                                 // initialize time to 0.0
    generate_event(B, SENDING, ARRIVE_LINK);     // initialize the event list
}

int loss_simulation(EVENT* newEvent) {
    if (jimsrand() < lossprob)  {
        nlost++;
        // loss 발생 시 추가된 이벤트를 지워줘야 한다. 단, 연결된 프레임을 분리시켜주고 지움. 타이머 인터럽트 이벤트에 들어있음
        newEvent->frmptr = NULL;
        deleteEvent(newEvent);
        return 1;
    }
    return 0;
}
int corruption_simulation(EVENT* newEvent) {
    float x;
    if (jimsrand() < corruptprob)  {
        ncorrupt++;
        if ( (x = jimsrand()) < .75)
            newEvent->frmptr->payload[0]='Z';     // CORRUPTED
        else if (x < .875)
            newEvent->frmptr->seqnum = 999999;    // DIFF SEQ
        else
            newEvent->frmptr->acknum = 999999;    // DIFF ACK
        return 1;
    }
    return 0;
}
void copy_frm(FRM* frm){
    int i;
    if(copied_frm !=NULL){
        free(copied_frm);
    }
    copied_frm = (FRM*)malloc(sizeof(FRM));
    copied_frm->seqnum = frm->seqnum;
    for(i=0; i<pkt_size; i++){
        copied_frm->payload[i] = frm->payload[i];
    }
}

void send_frm_to_B(EVENT *eventptr, FRM* frm2give, int re){  // re==1: 재전송
    int i;
    EVENT *newEvent;
    num_just_sent+=1;
    
    if(re!=1){
        newEvent = generate_event(B, RECEIVING, UPDATE);
        copy_frm(frm2give);
        newEvent->frmptr = frm2give;
        //printf("%.3lf: A has sent a frame(#%d) to B.\n", ttime, frm2give->seqnum);
    }
    else{  // 재전송
        //printf("%.3lf: A has re-transmitted the frame(#%d) %d-times.\n", ttime, copied_frm->seqnum, continuous_retran);
        frm2give = (FRM*)malloc(sizeof(FRM));
        frm2give->seqnum = copied_frm->seqnum;              // 복제해놓은 것은 또 다시 전송될 수 있으니
        for(i=0; i<pkt_size; i++){                           // 이를 다시 복사해와서 전달한다
            frm2give->payload[i] = copied_frm->payload[i];
        }
        newEvent = generate_event(B, RECEIVING, UPDATE);   // EVENT에 생성
        newEvent->frmptr = frm2give;                          // 생성한 EVENT에 dummy frame 부착
    }
    // A가 전송할 때 타이머를 시작
    startTimer(frm2give);
    
    // LOSS 경우, newEvent를 지워버린다.
    if(loss_simulation(newEvent)){
        //printf("WARNING: frame loss.\n");
        return;
    }
    // CORRUPTION
    if(corruption_simulation(newEvent)){
        //printf("WARNING: frame corruption\n");
        return;
    }
    
}
void send_ack(EVENT *eventptr){
    EVENT* newEvent;
    // A에게 ACK을 보냄
    newEvent = generate_event(A, RECEIVING, UPDATE);
    newEvent->frmptr = eventptr->frmptr;
    //printf("%.3lf: B has returned an ACK(#%d) to A.\n", ttime, CURRENT_ACK);
    
    // LOSS 경우, newEvent를 지워버린다.
    if(loss_simulation(newEvent)){
        //printf("WARNING: LOST ACK\n");
        return;
    }
    // CORRUPTION
    if(corruption_simulation(newEvent)){
        //printf("WARNING: CORRUPTED ACK\n");
    }
}
int retransmit_frm(EVENT* eventptr){  // 0: 그만 멈춤, 1: 계속 진행
    FRM *frm2give;
    continuous_retran++;                           // 재전송 횟수 카운트
    if(continuous_retran>retran_limit){          // 5번 이상 재전송 시 drop the frame
        //printf("WARNING DROPPED: FRAME(#=%d) has been dropped!\n", eventptr->frmptr->seqnum);  // 현재 drop되면 멈춤
        continuous_retran=0;
        if(nFraming == nFramingMax){
            return 0;
        }
        // 다음 frame 전송
        CURRENT_SEQ++;
        frm2give = framing_dgm();
        send_frm_to_B(eventptr, frm2give, 0);
        return 1;
    }
    else{
        send_frm_to_B(eventptr, copied_frm, 1);
        return 1;
    }
}

void unframe_B(EVENT* eventptr){
    int i;
    // B가 받은 frame을 unframing
    if(dgm_received!=NULL){
        free(dgm_received);
    }
    dgm_received = (DGM*)malloc(sizeof(DGM));
    for(i=0; i<pkt_size; i++){
        dgm_received->data[i] = eventptr->frmptr->payload[i];
    }
    //printf("%.3lf: B has unframed the frame(#%d).\n", ttime, eventptr->frmptr->seqnum);
}
void receive_frm(EVENT *eventptr){
    FRM *frm2give;
    EVENT *newEvent;
    
    // corrupted를 받음
    if(eventptr->frmptr->payload[0]=='Z'){
        //printf("\n%.3lf: B has received the corrupted frame(#%d).\n", ttime, eventptr->frmptr->seqnum);
    }
        // 올바른 SEQ를 받음
    else if(eventptr->frmptr->seqnum == CURRENT_ACK){
        //printf("\n%.3lf: B has received the safe frame(#%d).\n", ttime, eventptr->frmptr->seqnum);
        num_correct_received++;
        CURRENT_ACK++;                                          // ACK 증가
        frm2give = (FRM*)malloc(sizeof(FRM));                  // ACK 위한 dummy frame 생성
        frm2give->acknum = CURRENT_ACK;                        // 증가된 ACK를 프레임에 기록
        newEvent = generate_event(A, SENDING, NOUPDATE);     // EVENT에 생성
        newEvent->frmptr = frm2give;                            // 생성한 EVENT에 dummy frame 부착
        unframe_B(eventptr);
    }
        // B가 이미 중복된 프레임을 또 받음
    else if(eventptr->frmptr->seqnum == CURRENT_ACK-1){
        //printf("\n%.3lf: Duplicate frame(#%d) has been received.\n", ttime, eventptr->frmptr->seqnum);
        frm2give = (FRM*)malloc(sizeof(FRM));
        frm2give->acknum = CURRENT_ACK;
        newEvent = generate_event(A, SENDING, NOUPDATE);
        newEvent->frmptr = frm2give;
    }
        // 이상한 SEQ를 받음.
    else{
        //printf("\n%.3lf: B has received the corrupted frame(#%d).\n", ttime, eventptr->frmptr->seqnum);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
    EVENT *eventptr=NULL, *newEvent=NULL;
    FRM  *frm2give=NULL;
    int time_over=0, total_sent=0, correctly_received=0;
    float throughput=0, resultTime=0, average_trans_time=0;
    
    init();
    A_init();
    B_init();
    
    //printf("%.3lf: A has started to send packets to B.\n", ttime);
    while (1) {
        if(evlist==NULL)
            break;
        eventptr = evlist;             // 다음 실행할 이벤트
        evlist = evlist->next;        // 실행할 이벤트 삭제
        if (evlist!=NULL)
            evlist->prev=NULL;
        
        // 시간 제한 고려
        if(eventptr->evtime > SIM_TIME){
            //printf("%d sec - TIME UP\n", SIM_TIME);
            time_over = 1;
            break;
        }
        
        // SENDING
        if(eventptr->evtype == SENDING){
            // A가 B로 SEQ 보냄
            if(eventptr->eventity == B){
                ttime = eventptr->evtime;
                SENT_TIMESTAMP = ttime;
                //printf("\n%.3lf: New datagram arrived at the link layer.\n", ttime);
                frm2give = framing_dgm();
                send_frm_to_B(eventptr, frm2give, 0);
            }
                // B가 A로 ACK 보냄
            else{
                send_ack(eventptr);
            }
        }
            
            // RECEIVING
        else if(eventptr->evtype==RECEIVING){
            ttime = eventptr->evtime;
            // B가 A로부터 SEQ 받음
            if(eventptr->eventity == B){
                receive_frm(eventptr);
            }
                // A가 B로부터 ACK를 받음
            else{
                // A가 올바른 ACK를 받음
                if(CURRENT_SEQ+1 == eventptr->frmptr->acknum){
                    stopTimer();
                    RECEIVED_TIMESTAMP = ttime;
                    TOTAL_TRANS_DELAY += RECEIVED_TIMESTAMP-SENT_TIMESTAMP;
                    //printf("\n%.3lf: A has received the correct ACK(#%d). Stop the timer.\n", ttime, eventptr->frmptr->acknum);
                    CURRENT_SEQ++;
                    continuous_retran = 0;                                     // 재전송 횟수를 리셋
                    
                    if(nFraming==nFramingMax){
                        break;
                    }
                    
                    newEvent = generate_event(B, SENDING, ARRIVE_LINK);            // EVENT에 생성
                }
                    // CORRUPTED ACK를 받고 재전송 함
                else{
                    // 잘못된 ACK를 받으면 그냥 가만히 둬서 timeout 시킴
                }
            }
        }
            // TIMEOUT
        else{
            ttime = eventptr->evtime;
            //printf("\n%.3lf: TIMEOUT\n", ttime);
            free(evlist);                       // 현재 진행하고 있는 이후 작업을 다 지움
            evlist = NULL;
            
            if(!retransmit_frm(eventptr)){
                break;
            }
        }
    }
    
    SIM_TIME/=1000;
    if(time_over==1)
        resultTime = SIM_TIME;
    else
        resultTime = ttime/1000;
    throughput = num_correct_received*pkt_size*8/resultTime;
    total_sent = num_just_sent*pkt_size;
    correctly_received = num_correct_received*pkt_size;
    average_trans_time = TOTAL_TRANS_DELAY/num_correct_received;
    printf("\n=======================SETTING=======================\n");
    printf("- SIMULATION TIME LIMIT      : %d sec\n", SIM_TIME);
    printf("- # OF PACKETS TO SEND       : %d packets\n", nFramingMax);
    printf("- LOSS/CORRUPTION PROBABILITY: %.0lf %%\n", lossprob*100);
    printf("-----------------------------------------------------\n");
    printf("- SIZE OF EACH PACKET        : %d bytes\n", BYTES_TO_BE_SENT);
    printf("- TIMEOUT SETTING            : %d msec\n", TIMEOUT_MSEC);
    printf("=======================SUMMARY=======================\n");
//    if(time_over==1)
//        printf("- SIMUATION DURATION(sec)    : %d sec\n", SIM_TIME);
//    else
    printf("- SIMUATION DURATION(sec)    : %.3lf sec\n", resultTime);
//    printf("- SEND TOTAL                 : %d bytes\n", total_sent);
    if(total_sent>M)
        printf("- SEND TOTAL                 : %.6lf MB\n", (float)total_sent/M);
    else if(total_sent>K)
        printf("- SEND TOTAL                 : %.3lf KB\n", (float)total_sent/K);
    else
        printf("- SEND TOTAL                 : %d B\n", total_sent);
    
    if(correctly_received>M)
        printf("- CORRECTLY RECEIVED         : %.6lf MB\n", (float)correctly_received/M);
    else if(correctly_received>K)
        printf("- CORRECTLY RECEIVED         : %.3lf KB\n", (float)correctly_received/K);
    else
        printf("- CORRECTLY RECEIVED         : %d B\n", correctly_received);
//    printf("- CORRECTLY RECEIVED         : %d bytes\n", correctly_received);
    if(throughput>M)
        printf("- THROUGHPUT                 : %.6lf Mbps\n", throughput/M);
    else if(throughput>K)
        printf("- THROUGHPUT                 : %.3lf Kbps\n", throughput/K);
    else
        printf("- THROUGHPUT                 : %.3lf bps\n", throughput);
    printf("- AVERAGE TRANSMISSION DELAY : %.3lf msec\n", average_trans_time);
    printf("=====================================================\n");
    return 0;
}
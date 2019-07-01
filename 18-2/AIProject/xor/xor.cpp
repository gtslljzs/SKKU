#include <iostream>
#include <cmath>
using namespace std;

#define LAYER1 0
#define LAYER2 1
#define INPUT 0
#define L1_WEIGHTS 1
#define L2_WEIGHTS 2
#define OUTPUT 3
#define DERIVATIVE 4
#define ERROR 5

class NEURALNET {
private:
    int **input;
    int b1;
    int b2;
    int *y;
    double learning_rate;
    double err;
    double *w1;
    double *w2;
    double *dw1;
    double *dw2;
    double **h;
    double *output;

public:
    void init( int training_data[4][2], int training_y[4], double l1_weights[6], double l2_weights[3] );
    void node( int where );
    void derivative( int where );
    void update();
    void training();
    void calculate();
    void print( int type );
};

void NEURALNET::init(  int training_data[4][2], int training_y[4], double l1_weights[6], double l2_weights[3] ) {
    // allocate and set input
    this->input = new int*[4];
    for( int i = 0; i < 4; i++ )
        this->input[i] = new int[2];
    
    for( int i = 0; i < 4; i++ )
        for( int j = 0; j < 2; j++ )
            this->input[i][j] = training_data[i][j];
    
    // set biases
    this->b1 = 1;
    this->b2 = 1;

    // set learning rate
    this->learning_rate = 10;

    // allocate and set target value
    this->y = new int[4];
    for( int i = 0; i < 4; i++ )
        this->y[i] = training_y[i];
    
    // allocate weights
    this->w1 = new double[6];
    for( int i = 0; i < 6; i++ )
        this->w1[i] = l1_weights[i];
    
    this->w2 = new double[3];
    for( int i = 0; i < 3; i++ )
        this->w2[i] = l2_weights[i];

    // allocate f(x)
    this->h = new double*[4];
    for( int i = 0; i < 4; i++ )
        this->h[i] = new double[2];
    
    // allocate output
    this->output = new double[4];

    // allocate derivative weights
    this->dw1 = new double[6];
    this->dw2 = new double[6];
}

void NEURALNET::node( int where ) {
    switch( where ) {
        case LAYER1:
            double* net1;
            net1 = new double[4];
            double* net2;
            net2 = new double[4];

            for( int i = 0; i < 4; i++ ) {
                net1[i] = 0;
                net2[i] = 0;
            }

            for( int n = 0; n < 4; n++ ) {
                for ( int i = 0; i < 2; i++ ) {
                    net1[n] += this->input[n][i] * this->w1[i];
                    net2[n] += this->input[n][i] * this->w1[i+3];
                }
                net1[n] += this->w1[2] * this->b1;
                net2[n] += this->w1[5] * this->b1;
            }

            for( int n = 0; n < 4; n++ ) {
                h[n][0] = 1 / ( 1 + exp( -net1[n] ) );
                h[n][1] = 1 / ( 1 + exp( -net2[n] ) );
            }

            delete[] net1;
            delete[] net2;
            break;

        case LAYER2:
            double* net;
            net = new double[4];
            for( int i = 0; i < 4; i++ )
                net[i] = 0;

            for( int n = 0; n < 4; n++ ) {
                for( int i = 0; i < 2; i++ ) {
                    net[n] += h[n][i] * w2[i];
                }
                net[n] += w2[2] * b2;
            }

            for( int n = 0; n < 4; n++ )
                output[n] = 1 / ( 1 + exp( -net[n] ) );

            delete[] net;
            break;

        default:
            break;
    }
}

void NEURALNET::derivative( int where ) {
    double** derivative;

    switch( where ) {
        case LAYER1:
            derivative = new double*[4];
            for( int n = 0; n < 4; n++ )
                derivative[n] = new double[6];

            for( int n = 0; n < 4; n++ ) {
                for( int i = 0; i < 2; i++ ) {
                    derivative[n][i] = -input[n][0] * h[n][0] * ( 1 - h[n][0] ) * w2[0] * ( y[n] - output[n] ) * output[n] * ( 1 - output[n] );
                    derivative[n][i + 3] = -input[n][1] * h[n][1] * ( 1 - h[n][1] ) * w2[1] * ( y[n] - output[n] ) * output[n] * ( 1 - output[n] );
                }
                derivative[n][2] = -b1 * h[n][0] * ( 1 - h[n][0] ) * w2[0] * ( y[n] - output[n] ) * output[n] * ( 1 - output[n] );
                derivative[n][5] = -b1 * h[n][1] * ( 1 - h[n][1] ) * w2[1] * ( y[n] - output[n] ) * output[n] * ( 1 - output[n] );
            }

            // sum derivatives
            for( int i = 0; i < 6; i++ )
                dw1[i] = 0;
            for( int n = 0; n < 4; n++ )
                for( int i = 0; i < 6; i++ )
                    dw1[i] += derivative[n][i];
            
            for( int n = 0; n < 4; n++ )
                delete[] derivative[n];
            delete[] derivative;

            break;
        
        case LAYER2:
            derivative = new double*[4];
            for( int n = 0; n < 4; n++ )
                derivative[n] = new double[3];

            for( int n = 0; n < 4; n++ ) {
                for( int i = 0; i < 2; i++ )
                    derivative[n][i] = -( y[n] - output[n] ) * output[n] * ( 1 - output[n] ) * h[n][i];
                derivative[n][2] = -( y[n] - output[n] ) * output[n] * ( 1 - output[n] ) * b2;
            }

            for( int i = 0; i < 3; i++ )
                dw2[i] = 0;
            for( int n = 0; n < 4; n++ )
                for( int i = 0; i < 3; i++ )
                    dw2[i] += derivative[n][i];

            for( int n = 0; n < 4; n++ )
                delete[] derivative[n];
            delete[] derivative;

            break;
        
        default:
            break;
    }
}

void NEURALNET::update() {
    for( int i = 0; i < 3; i++ )
        w2[i] -= this->learning_rate * dw2[i];

    for( int i = 0; i < 6; i++ )
        w1[i] -= this->learning_rate * dw1[i];
}

void NEURALNET::training() {
    int cnt = 0;
    int num_train;

    cout << "How many times do you want to train?: ";
    scanf( "%d", &num_train );

    while( cnt != num_train ) {
        node( LAYER1 );
        node( LAYER2 );
        derivative( LAYER2 );
        derivative( LAYER1 );
        update();

        cnt++;
    }
}

void NEURALNET::calculate() {
    int input[2];
    int bias1 = 1;
    int bias2 = 1;
    double sum1[2];
    double fx[2];
    double sum2 = 0;
    double result;

    cout << "Enter the input ( only 0 or 1 ): ";
    scanf( "%d %d", &input[0], &input[1] );
    cout << endl;

    if( !( input[0] == 0 || input[0] == 1 ) ) {
        cout << "Invalid input" << endl;
        return;
    } else if( !( input[1] == 0 || input[1] == 1 ) ) {
        cout << "Invalid input" << endl;
        return;
    }

    for( int i = 0; i < 2; i++ )
        sum1[i] = 0;
    for( int i = 0; i < 2; i++ ) {
        sum1[0] += input[i] * this->w1[i];
        sum1[1] += input[i] * this->w1[i + 3];
    }
    sum1[0] += bias1 * this->w1[2];
    sum1[1] += bias1 * this->w1[5];

    for( int i = 0; i < 2; i++ )
        fx[i] = 1 / ( 1 + exp( -sum1[i] ) );

    for( int i = 0; i < 2; i++ )
        sum2 += fx[i] * w2[i];
    sum2 += bias2 * w2[2];

    result = 1 / ( 1 + exp( -sum2 ) );

    cout << "result: " << result << endl;
}

void NEURALNET::print( int type ) {
    switch( type ) {
        case ERROR:
            this->err = 0;
            double *en;
            en = new double[4];
            for( int i = 0; i < 4; i++ )
                en[i] = 0;
            
            for( int i = 0; i < 4; i++ )
                en[i] = pow( (double)this->y[i] - this->output[i], 2 ) / 2;
            
            for( int i = 0; i < 4; i++ )
                this->err += en[i];
            
            printf( "E(x) = %.5f\n", this->err );

            break;

        case INPUT:
            for( int i = 0; i < 4; i++ ) {
                for( int j = 0; j < 2; j++ )
                    printf( "%d ", this->input[i][j] );
                cout << endl;
            }
            cout << endl;
            break;

        case L1_WEIGHTS:
            cout << "< L1 weights >" << endl;
            for( int i = 0; i < 6; i++ )
                printf( "%.5lf ", this->w1[i] );
            cout << endl;
            break;

        case L2_WEIGHTS:
            cout << "< L2 weights >" << endl;
            for( int i = 0; i < 3; i++ )
                printf( "%.5lf ", this->w2[i] );
            cout << endl << endl;
            break;

        case OUTPUT:
            for( int i = 0; i < 4; i++ )
                printf( "%.5lf\n", this->output[i] );
            cout << endl;
            break;

        case DERIVATIVE:
            cout << "< layer 2 derivatives >" << endl;
            for( int i = 0; i < 3; i++ )
                printf( "%.5lf ", dw2[i] );
            cout << endl << endl;
            
            cout << "< layer 1 derivatives >" << endl;
            for( int i = 0; i < 6; i++ )
                printf( "%.5lf ", dw1[i] );
            cout << endl << endl;

        default:
            break;
    }
}

int main() {
    int training_data[4][2] = { {1, 1}, {1, 0}, {0, 1}, {0, 0} };
    int training_y[4] = {0, 1, 1, 0};
    double l1_weights[6] = { -0.089, 0.028, 0.092, 0.098, -0.07, -0.01 };
    double l2_weights[3] = { 0.056, 0.067, 0.016 };

    NEURALNET nn;

    nn.init( training_data, training_y, l1_weights, l2_weights );
    nn.training();
    nn.print( L1_WEIGHTS );
    nn.print( L2_WEIGHTS );
    nn.print( ERROR );

    nn.calculate();

    return 0;
}

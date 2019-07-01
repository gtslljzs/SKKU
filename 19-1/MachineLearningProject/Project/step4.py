# step3_output파일과 딕셔너리 파일을 읽어서 기사의 문장을 ID로 매핑하는 코드
import pickle
import re


if __name__ == "__main__":
    print('Start!!!')

    # 한 문장 최대 길이
    max_len = 200

    # 딕셔너리 파일을 호출
    with open("step3_output.pickle", 'rb') as f:
        word_dict = pickle.load(f)

    # X, Y 값을 모두 하나의 리스트에 넣겠다.
    # Train_Set[i]는 튜플
    # Train_set[i][0] = X값
    # Train_Set[i][1] = Y값
    Train_Set = list()

    with open("step3_output.csv", 'r') as f:
        # Train 데이터 X
        with open("step4_output_X.csv", 'w') as X:
            # Train 데이터 Y
            with open("step4_output_Y.csv", 'w') as Y:
                page = 0
                while True:
                    page = page + 1
                    line = f.readline()
                    if not line: break

                    print("\n<<< Page %d >>>" % page)
                    temp_list = re.split('[,]+', line)  # [0]은 X값, [1]은 Y값

                    x_list = re.split('[ \n]+', temp_list[0])
                    y_list = re.split('[ \n]+', temp_list[1])

                    # 사전에 없는 단어는 <unk>로 처리
                    x_list = [word_dict.get(w, word_dict["<unk>"]) for w in x_list]
                    y_list = [word_dict.get(w, word_dict["<unk>"]) for w in y_list]
                    # max_len 길이만큼으로 <padding>을 추가해서 X 구성. Y는 따로 길이 추가 안함.
                    x_len = len(x_list)
                    x_list = x_list[:max_len]
                    if x_len < max_len :
                        for i in range(x_len, max_len):
                            x_list.append(word_dict["<padding>"])

                    print("### X ###\n", x_list)
                    print("### Y ###\n", y_list)

                    # Train_Set에 하나씩 저장
                    Train_Set.append((x_list,y_list))

    # 파일로 쓰기
    with open("step4_output.pickle", 'wb') as t:
        pickle.dump(Train_Set, t)

    # 확인용 테스트 코드
    # with open("step4_output.pickle", 'rb') as f:
    #     Temp_set = pickle.load(f)
    # print("<<< Temp_set[0] >>>")
    # print(Temp_set[0])
    # print("length = ", len(Temp_set[0][0]))

    print('Finish!!!')

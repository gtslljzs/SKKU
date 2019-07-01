/* Choi Bongguen, 2013311689 */

#include <stdio.h>
#include <string.h>

typedef struct {
    unsigned char data[16];
} super;

// Operation function declare
super add_super(double, double);
super mul_super(double, double);

// Print function declare
void print_bit(super);
void print_norm(super);

int main() {
    double op1, op2;

    printf("Input two double precision values: \n");
    scanf("%lf %lf", &op1, &op2);

    printf("Addition (Bit Sequence): ");
    print_bit(add_super(op1, op2));
    printf("\n");
    printf("Addition (Normalized): ");
    print_norm(add_super(op1, op2));
    printf("\n");

    printf("Multiplication (Bit Sequence): ");
    print_bit(mul_super(op1, op2));
    printf("\n");
    printf("Multiplication (Normalized): ");
    print_norm(mul_super(op1, op2));
    printf("\n");

    return 0;
}

// Operation function define
super add_super(double op1, double op2) {
    super result_bit;
    double result;
    long long int cast, op1_cast, op2_cast;
    unsigned short s_bias = 16383;
    unsigned short d_bias = 1023;
    unsigned short op1_exp = 0, op2_exp = 0, r_exp = 0, s_exp = 0;
    long long unsigned op1_frac = 0, op2_frac = 0, s_frac = 0;

    for (int i = 0; i < 16; i++)
        result_bit.data[i] = 0;

    if ((op1 -1.79e308 + op2 <= 0) && (op1 + op2 >= 0)) { // 0 <= op1 + op2 <= 1.79e308
        result = op1 + op2;
        memcpy(&cast, &result, sizeof(double));

        // copying sign and exponential part
        unsigned char tmp = 0;
        r_exp = cast >> 52;
        if (r_exp == 0)
            s_exp == 0;
        else
            s_exp = r_exp - d_bias + s_bias;
        memcpy(&result_bit.data[1], &s_exp, sizeof(char));
        tmp = s_exp >> 8;
        memcpy(&result_bit.data[0], &tmp, sizeof(char));

        // copying fractional part
        tmp = cast >> 44;
        memcpy(&result_bit.data[2], &tmp, sizeof(char));
        tmp = cast >> 36;
        memcpy(&result_bit.data[3], &tmp, sizeof(char));
        tmp = cast >> 28;
        memcpy(&result_bit.data[4], &tmp, sizeof(char));
        tmp = cast >> 20;
        memcpy(&result_bit.data[5], &tmp, sizeof(char));
        tmp = cast >> 12;
        memcpy(&result_bit.data[6], &tmp, sizeof(char));
        tmp = cast >> 4;
        memcpy(&result_bit.data[7], &tmp, sizeof(char));
        tmp = cast << 4;
        memcpy(&result_bit.data[8], &tmp, sizeof(char));

    } else if ((op1 + 1.79e308 + op2 >= 0) && (op1 + op2 < 0)) { // -1.79e308 <= op1 + op2 < 0
        result = op1 + op2;
        memcpy(&cast, &result, sizeof(double));

        // copying sign and exponential part
        unsigned short i = 1;
        unsigned char tmp = 0;
        r_exp = (cast >> 52) - (i << 15) - (i << 14) - (i << 13) - (i << 12) - (i << 11);
        if (r_exp == 0)
            s_exp == 0;
        else
            s_exp = r_exp - d_bias + s_bias;
        memcpy(&result_bit.data[1], &s_exp, sizeof(char));
        tmp = (s_exp >> 8) + 128;
        memcpy(&result_bit.data[0], &tmp, sizeof(char));

        // copying fractional part
        tmp = cast >> 44;
        memcpy(&result_bit.data[2], &tmp, sizeof(char));
        tmp = cast >> 36;
        memcpy(&result_bit.data[3], &tmp, sizeof(char));
        tmp = cast >> 28;
        memcpy(&result_bit.data[4], &tmp, sizeof(char));
        tmp = cast >> 20;
        memcpy(&result_bit.data[5], &tmp, sizeof(char));
        tmp = cast >> 12;
        memcpy(&result_bit.data[6], &tmp, sizeof(char));
        tmp = cast >> 4;
        memcpy(&result_bit.data[7], &tmp, sizeof(char));
        tmp = cast << 4;
        memcpy(&result_bit.data[8], &tmp, sizeof(char));

    } else if (op1 -1.79e308 + op2 > 0) {                  // overflow
        unsigned short exp_dif;

        memcpy(&op1_cast, &op1, sizeof(double));
        memcpy(&op2_cast, &op2, sizeof(double));

        op1_exp = op1_cast >> 52;
        op2_exp = op2_cast >> 52;

        exp_dif = (op1_exp > op2_exp ? op1_exp : op2_exp) - (op1_exp > op2_exp ? op2_exp : op1_exp);

        if (exp_dif == 0) {
            unsigned char tmp = 0, tmp1 = 0, tmp2 = 0;
            long long unsigned tmp_frac;
            unsigned short count = 0;

            op1_frac = 1, op2_frac = 1;
            op1_frac = op1_frac << 8;
            op2_frac = op2_frac << 8;
            for (int i = 0; i < 6; i++) {
                tmp1 = op1_cast >> (44 - i * 8);
                tmp2 = op2_cast >> (44 - i * 8);
                memcpy(&op1_frac, &tmp1, sizeof(char));
                memcpy(&op2_frac, &tmp2, sizeof(char));
                op1_frac = op1_frac << 8;
                op2_frac = op2_frac << 8;
            }
            tmp1 = op1_cast << 4;
            tmp2 = op2_cast << 4;
            memcpy(&op1_frac, &tmp1, sizeof(char));
            memcpy(&op2_frac, &tmp2, sizeof(char));
            op1_frac = op1_frac >> 4;
            op2_frac = op2_frac >> 4;

            s_frac = op1_frac + op2_frac;
            tmp_frac = s_frac;
            tmp_frac = tmp_frac >> 52;

            while (tmp_frac != 1) {
                count++;
                tmp_frac = tmp_frac >> 1;
            }

            // copying sign and exponential part
            s_exp = op1_exp + count - d_bias + s_bias;
            memcpy(&result_bit.data[1], &s_exp, sizeof(char));
            tmp = s_exp >> 8;
            memcpy(&result_bit.data[0], &tmp, sizeof(char));

            // copying fractional part
            tmp = s_frac >> (44 + count);
            memcpy(&result_bit.data[2], &tmp, sizeof(char));
            tmp = s_frac >> (36 + count);
            memcpy(&result_bit.data[3], &tmp, sizeof(char));
            tmp = s_frac >> (28 + count);
            memcpy(&result_bit.data[4], &tmp, sizeof(char));
            tmp = s_frac >> (20 + count);
            memcpy(&result_bit.data[5], &tmp, sizeof(char));
            tmp = s_frac >> (12 + count);
            memcpy(&result_bit.data[6], &tmp, sizeof(char));
            tmp = s_frac >> (4 + count);
            memcpy(&result_bit.data[7], &tmp, sizeof(char));

            if (count <= 4) {
                tmp = s_frac << (4 - count);
                memcpy(&result_bit.data[8], &tmp, sizeof(char));
            } else {
                int i = 0;
                int n_count = count;
                while (n_count > 4) {
                    tmp = s_frac >> (n_count - 4);
                    memcpy(&result_bit.data[8 + i], &tmp, sizeof(char));
                    i++;
                    n_count -= 8;
                }
                tmp = s_frac << (12 - count);
                memcpy(&result_bit.data[8 + i], &tmp, sizeof(char));
            }
        } else {
            unsigned char tmp = 0, tmp1 = 0, tmp2 = 0;
            long long unsigned tmp_frac;
            unsigned short count = 0;

            op1_frac = 1, op2_frac = 1;
            op1_frac = op1_frac << 8;
            op2_frac = op2_frac << 8;
            for (int i = 0; i < 6; i++) {
                tmp1 = op1_cast >> (44 - i * 8);
                tmp2 = op2_cast >> (44 - i * 8);
                memcpy(&op1_frac, &tmp1, sizeof(char));
                memcpy(&op2_frac, &tmp2, sizeof(char));
                op1_frac = op1_frac << 8;
                op2_frac = op2_frac << 8;
            }
            tmp1 = op1_cast << 4;
            tmp2 = op2_cast << 4;
            memcpy(&op1_frac, &tmp1, sizeof(char));
            memcpy(&op2_frac, &tmp2, sizeof(char));
            op1_frac = op1_frac >> 4;
            op2_frac = op2_frac >> 4;

            if (op1_exp > op2_exp)
                op1_frac = op1_frac << exp_dif;
            else
                op2_frac = op2_frac << exp_dif;

            s_frac = op1_frac + op2_frac;
            tmp_frac = s_frac;
            tmp_frac = tmp_frac >> (52 + exp_dif);

            while (tmp_frac != 1) {
                count++;
                tmp_frac = tmp_frac >> 1;
            }

            // copying sign and exponential part
            if (op1_exp > op2_exp) {
                s_exp = op1_exp + count - d_bias + s_bias;
                memcpy(&result_bit.data[1], &s_exp, sizeof(char));
                tmp = s_exp >> 8;
                memcpy(&result_bit.data[0], &tmp, sizeof(char));
            } else {
                s_exp = op2_exp + count - d_bias + s_bias;
                memcpy(&result_bit.data[1], &s_exp, sizeof(char));
                tmp = s_exp >> 8;
                memcpy(&result_bit.data[0], &tmp, sizeof(char));
            }

            // copying fractional part
            tmp = s_frac >> (44 + exp_dif + count);
            memcpy(&result_bit.data[2], &tmp, sizeof(char));
            tmp = s_frac >> (36 + exp_dif + count);
            memcpy(&result_bit.data[3], &tmp, sizeof(char));
            tmp = s_frac >> (28 + exp_dif + count);
            memcpy(&result_bit.data[4], &tmp, sizeof(char));
            tmp = s_frac >> (20 + exp_dif + count);
            memcpy(&result_bit.data[5], &tmp, sizeof(char));
            tmp = s_frac >> (12 + exp_dif + count);
            memcpy(&result_bit.data[6], &tmp, sizeof(char));
            tmp = s_frac >> (4 + exp_dif + count);
            memcpy(&result_bit.data[7], &tmp, sizeof(char));

            if (exp_dif + count <= 4) {
                tmp = s_frac << (4 - (count + exp_dif));
                memcpy(&result_bit.data[8], &tmp, sizeof(char));
            } else {
                int i = 0;
                int n_count = count + exp_dif;
                while (n_count <= 4) {
                    tmp = s_frac >> (n_count - 4);
                    memcpy(&result_bit.data[8 + i], &tmp, sizeof(char));
                    i++;
                    n_count -= 8;
                }
                tmp = s_frac << (12 - (count + exp_dif));
                memcpy(&result_bit.data[8 + i], &tmp, sizeof(char));
            }
        }
    } else if (op1 + 1.79e308 + op2 < 0) {                        // underflow
        unsigned short exp_dif;
        unsigned short i = 1;

        memcpy(&op1_cast, &op1, sizeof(double));
        memcpy(&op2_cast, &op2, sizeof(double));

        op1_exp = (cast >> 52) - (i << 15) - (i << 14) - (i << 13) - (i << 12) - (i << 11);
        op2_exp = (cast >> 52) - (i << 15) - (i << 14) - (i << 13) - (i << 12) - (i << 11);
        exp_dif = (op1_exp > op2_exp ? op1_exp : op2_exp) - (op1_exp > op2_exp ? op2_exp : op1_exp);

        if (exp_dif == 0) {
            unsigned char tmp = 0, tmp1 = 0, tmp2 = 0;
            long long unsigned tmp_frac;
            unsigned short count = 0;

            op1_frac = 1, op2_frac = 1;
            op1_frac = op1_frac << 8;
            op2_frac = op2_frac << 8;
            for (int i = 0; i < 6; i++) {
                tmp1 = op1_cast >> (44 - i * 8);
                tmp2 = op2_cast >> (44 - i * 8);
                memcpy(&op1_frac, &tmp1, sizeof(char));
                memcpy(&op2_frac, &tmp2, sizeof(char));
                op1_frac = op1_frac << 8;
                op2_frac = op2_frac << 8;
            }
            tmp1 = op1_cast << 4;
            tmp2 = op2_cast << 4;
            memcpy(&op1_frac, &tmp1, sizeof(char));
            memcpy(&op2_frac, &tmp2, sizeof(char));
            op1_frac = op1_frac >> 4;
            op2_frac = op2_frac >> 4;

            s_frac = op1_frac + op2_frac;
            tmp_frac = s_frac;
            tmp_frac = tmp_frac >> 52;

            while (tmp_frac != 1) {
                count++;
                tmp_frac = tmp_frac >> 1;
            }

            // copying sign and exponential part
            s_exp = op1_exp + count - d_bias + s_bias;
            memcpy(&result_bit.data[1], &s_exp, sizeof(char));
            tmp = (s_exp >> 8) + 128;
            memcpy(&result_bit.data[0], &tmp, sizeof(char));

            // copying fractional part
            tmp = s_frac >> (44 + count);
            memcpy(&result_bit.data[2], &tmp, sizeof(char));
            tmp = s_frac >> (36 + count);
            memcpy(&result_bit.data[3], &tmp, sizeof(char));
            tmp = s_frac >> (28 + count);
            memcpy(&result_bit.data[4], &tmp, sizeof(char));
            tmp = s_frac >> (20 + count);
            memcpy(&result_bit.data[5], &tmp, sizeof(char));
            tmp = s_frac >> (12 + count);
            memcpy(&result_bit.data[6], &tmp, sizeof(char));
            tmp = s_frac >> (4 + count);
            memcpy(&result_bit.data[7], &tmp, sizeof(char));

            if (count <= 4) {
                tmp = s_frac << (4 - count);
                memcpy(&result_bit.data[8], &tmp, sizeof(char));
            } else {
                int i = 0;
                int n_count = count;
                while (n_count > 4) {
                    tmp = s_frac >> (n_count - 4);
                    memcpy(&result_bit.data[8 + i], &tmp, sizeof(char));
                    i++;
                    n_count -= 8;
                }
                tmp = s_frac << (12 - count);
                memcpy(&result_bit.data[8 + i], &tmp, sizeof(char));
            }
        } else {
            unsigned char tmp = 0, tmp1 = 0, tmp2 = 0;
            long long unsigned tmp_frac;
            unsigned short count = 0;

            op1_frac = 1, op2_frac = 1;
            op1_frac = op1_frac << 8;
            op2_frac = op2_frac << 8;
            for (int i = 0; i < 6; i++) {
                tmp1 = op1_cast >> (44 - i * 8);
                tmp2 = op2_cast >> (44 - i * 8);
                memcpy(&op1_frac, &tmp1, sizeof(char));
                memcpy(&op2_frac, &tmp2, sizeof(char));
                op1_frac = op1_frac << 8;
                op2_frac = op2_frac << 8;
            }
            tmp1 = op1_cast << 4;
            tmp2 = op2_cast << 4;
            memcpy(&op1_frac, &tmp1, sizeof(char));
            memcpy(&op2_frac, &tmp2, sizeof(char));
            op1_frac = op1_frac >> 4;
            op2_frac = op2_frac >> 4;

            if (op1_exp > op2_exp)
                op1_frac = op1_frac << exp_dif;
            else
                op2_frac = op2_frac << exp_dif;

            s_frac = op1_frac + op2_frac;
            tmp_frac = s_frac;
            tmp_frac = tmp_frac >> (52 + exp_dif);

            while (tmp_frac != 1) {
                count++;
                tmp_frac = tmp_frac >> 1;
            }

            // copying sign and exponential part
            if (op1_exp > op2_exp) {
                s_exp = op1_exp + count - d_bias + s_bias;
                memcpy(&result_bit.data[1], &s_exp, sizeof(char));
                tmp = (s_exp >> 8) + 128;
                memcpy(&result_bit.data[0], &tmp, sizeof(char));
            } else {
                s_exp = op2_exp + count - d_bias + s_bias;
                memcpy(&result_bit.data[1], &s_exp, sizeof(char));
                tmp = (s_exp >> 8) + 128;
                memcpy(&result_bit.data[0], &tmp, sizeof(char));
            }

            // copying fractional part
            tmp = s_frac >> (44 + exp_dif + count);
            memcpy(&result_bit.data[2], &tmp, sizeof(char));
            tmp = s_frac >> (36 + exp_dif + count);
            memcpy(&result_bit.data[3], &tmp, sizeof(char));
            tmp = s_frac >> (28 + exp_dif + count);
            memcpy(&result_bit.data[4], &tmp, sizeof(char));
            tmp = s_frac >> (20 + exp_dif + count);
            memcpy(&result_bit.data[5], &tmp, sizeof(char));
            tmp = s_frac >> (12 + exp_dif + count);
            memcpy(&result_bit.data[6], &tmp, sizeof(char));
            tmp = s_frac >> (4 + exp_dif + count);
            memcpy(&result_bit.data[7], &tmp, sizeof(char));

            if (exp_dif + count <= 4) {
                tmp = s_frac << (4 - (count + exp_dif));
                memcpy(&result_bit.data[8], &tmp, sizeof(char));
            } else {
                int i = 0;
                int n_count = count + exp_dif;
                while (n_count <= 4) {
                    tmp = s_frac >> (n_count - 4);
                    memcpy(&result_bit.data[8 + i], &tmp, sizeof(char));
                    i++;
                    n_count -= 8;
                }
                tmp = s_frac << (12 - (count + exp_dif));
                memcpy(&result_bit.data[8 + i], &tmp, sizeof(char));
            }
        }
    }

    return result_bit;
}
super mul_super(double op1, double op2) {
    super result;
    long long int op1_cast, op2_cast;
    int sign;
    unsigned short op1_exp, op2_exp, s_exp;
    unsigned short d_bias = 1023;
    unsigned short s_bias = 16383;
    long long unsigned op1_frac, op2_frac, ll_tmp;

    for (int i = 0; i < 16; i++)
        result.data[i] = 0;

    if (op1 == 0 || op2 == 0)
        return result;
    else {
        memcpy(&op1_cast, &op1, sizeof(double));
        memcpy(&op2_cast, &op2, sizeof(double));

        if ((op1 >= 0 && op2 >= 0) || (op1 < 0 && op2 < 0))
            sign = 0;
        else
            sign = 1;

        // op1 exponential and fractional part
        if (op1 < 0) {
            long long int i = 1;
            op1_cast = op1_cast - (i << 63);
        }
        op1_exp = op1_cast >> 52;
        ll_tmp = op1_exp;
        ll_tmp = ll_tmp << 52;
        op1_frac = op1_cast - ll_tmp;
        ll_tmp = 1;
        ll_tmp = ll_tmp << 52;
        op1_frac = op1_frac + ll_tmp;

        // op2 exponential and fractional part
        if (op2 < 0) {
            long long int i = 1;
            op2_cast = op2_cast - (i << 63);
        }
        op2_exp = op2_cast >> 52;
        ll_tmp = op2_exp;
        ll_tmp = ll_tmp << 52;
        op2_frac = op2_cast - ll_tmp;
        ll_tmp = 1;
        ll_tmp = ll_tmp << 52;
        op2_frac = op2_frac + ll_tmp;

        // super exponent
        s_exp = op1_exp + op2_exp - 2 * d_bias + s_bias;
        if (sign == 1) {
            unsigned short i = 1;
            s_exp += (i << 15);
        }
        memcpy(&result.data[1], &s_exp, sizeof(char));
        s_exp = s_exp >> 8;
        memcpy(&result.data[0], &s_exp, sizeof(char));

        // multiplication
        unsigned char multiplier;
        long long unsigned mul_result = 0, l_store = 0;

        // make low order
        for (int i = 0; i < 8; i++) {
            multiplier = (op2_frac >> (8 * i));
            mul_result += op1_frac * multiplier;
            memcpy(&l_store, &mul_result, sizeof(char));
            l_store = l_store << 8;
            mul_result = mul_result >> 8;
        }

        // storing high order
        int count = 0;
        int idx = 2;
        unsigned char c_tmp;

        ll_tmp = mul_result;
        while (ll_tmp != 1) {
            ll_tmp = ll_tmp >> 1;
            count++;
        }

        while (count >= 0) {
            c_tmp = mul_result >> (count - 8);
            result.data[idx] = c_tmp;
            idx++;
            count -= 8;
        }

        // storing low order
        count += 8;

        c_tmp = mul_result << (8 - count);
        c_tmp += l_store >> (56 + count);
        result.data[idx] = c_tmp;
        idx++;

        for (int i = 0; i < 7; i++) {
            c_tmp = l_store >> (48 + count - i * 8);
            result.data[idx] = c_tmp;
            idx++;
        }

        c_tmp = l_store << (8 - count);
        result.data[idx] = c_tmp;
    }

    return result;
}

// Print function define
void print_bit(super result) {
    for (int i = 0; i < 16; i++) {
        int cast = 0;
        int multi = 1;

        if (result.data[15 - i] < 2)
            printf("0000000");
        else if (result.data[15 - i] < 4)
            printf("000000");
        else if (result.data[15 - i] < 8)
            printf("00000");
        else if (result.data[15 - i] < 16)
            printf("0000");
        else if (result.data[15 - i] < 32)
            printf("000");
        else if (result.data[15 - i] < 64)
            printf("00");
        else if (result.data[15 - i] < 128)
            printf("0");

        for (int j = 0; j < 8; j++) {
            if (result.data[15 - i] == 0)
                break;
            int remainder = 0;

            remainder = result.data[15 - i] % 2;
            cast += remainder * multi;
            result.data[15 - i] /= 2;
            multi *= 10;
        }
        printf("%d ", cast);
    }
}
void print_norm(super result) { // result = big endian
    int bias = 16383;
    int E;
    unsigned short exp = 0;

    if (result.data[0] >= 128) {
        exp = result.data[0] - 128;
        printf("-");
    } else
        exp = result.data[0];

    exp = exp << 8;
    memcpy(&exp, &result.data[1], sizeof(char));

    if (exp == 0)
        E = 1 - bias;
    else
        E = exp - bias;

    if (E == 1 - bias)
        printf("0.");
    else
        printf("1.");

    for (int i = 2; i < 16; i++) {
        int cast = 0;
        int multi = 1;

        if (result.data[i] < 2)
            printf("0000000");
        else if (result.data[i] < 4)
            printf("000000");
        else if (result.data[i] < 8)
            printf("00000");
        else if (result.data[i] < 16)
            printf("0000");
        else if (result.data[i] < 32)
            printf("000");
        else if (result.data[i] < 64)
            printf("00");
        else if (result.data[i] < 128)
            printf("0");

        for (int j = 0; j < 8; j++) {
            if (result.data[i] == 0)
                break;
            int remainder = 0;

            remainder = result.data[i] % 2;
            cast += remainder * multi;
            result.data[i] /= 2;
            multi *= 10;
        }
        printf("%d", cast);
    }

    printf(" x 2^%d", E);

}

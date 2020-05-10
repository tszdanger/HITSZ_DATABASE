#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "extmem.h"
#define MAX 1000
typedef struct index  //����
{
    int a;
    int blk;
} index;
void PRINTBLK(unsigned char *blk);
void GETBLKTEM(unsigned char *blk, int k, int *x, int *y);
void SETBLKTEM(unsigned char *blk, int k, int x, int y);
void GETBLKTEM_MUL(unsigned char *blk, int k, int *x, int *y, int valid);
void SORTTEM(int tem[][2], int row);
void MULSORT(int mul_num, int total, int begin, int outputnum, Buffer *buf);
int TRICOM(int a, int b, int c, int a1, int b1, int c1);
int TRICOM_RES(int a, int b, int c, int a1, int b1, int c1, int *resultx,
               int *resulty);
//ָ����·����,begin��Blk��ʼ�ĺ�����outputnum�������blk������total���ܹ��Ŀ���

//����������addr�ǹ�ϵ��ʼ�ĺ�����len���ܵĹ�ϵ�ĸ���,outaddr���������ڿ��ϵĺ���
index *CreateIndex(Buffer *buf, int addr, int outaddr, int len);

index *CreateIndex(Buffer *buf, int addr, int outaddr, int len) {
    index *myindex = malloc(sizeof(index) * 40);  //��40������
    // r��addr��200
    // s��addr��400
    int i, j, k = 0;
    int index_num = 0;
    unsigned char *blk, *temp_blk;
    int tempx, tempy;
    int oldtempx, oldtempy;
    int line_count;
    int blk_num = len / 7;  // R����16��S��32
    //��Ҫ�ѹ�ϵ����Ķ���������
    int tem_array1[112][2] = {0};
    int tem_array2[224][2] = {0};
    int tc_size = 0;
    temp_blk = getNewBlockInBuffer(buf);

    if (len == 112) {
        outaddr = 600;  // 600��ʼ��r����
    } else {
        outaddr = 650;  // 650��ʼ��s����
    }
    //��������215��
    // blk = readBlockFromDisk(215, buf);
    // GETBLKTEM(blk, 5, &tempx, &tempy);
    // printf("�������Խ��%d, %d ",tempx,tempy);
    // freeBlockInBuffer(blk, buf);
    // tempx = 0;
    // tempy = 0;

    if (len == 112) {
        //����Ƕ�r�Ľ�������
        for (i = 1; i < 17; i++) {
            blk = readBlockFromDisk(i - 1 + addr, buf);
            printf("�������ݿ�%d\n", i - 1 + addr);
            for (j = 0; j < 7; j++) {
                GETBLKTEM(blk, j, &tempx, &tempy);
                // printf("%d, %d ",tempx,tempy);
                tem_array1[(i - 1) * 7 + j][0] = tempx;
                tem_array1[(i - 1) * 7 + j][1] = tempy;
                //������Ӧ�ñ߶����ݱ߽�������
                //����ǵ�һ��
                if (i == 1 && j == 0) {
                    myindex[index_num].a = tempx;
                    myindex[index_num].blk = i + addr - 1;
                    oldtempx = tempx;
                    SETBLKTEM(temp_blk, tc_size, myindex[index_num].a,
                              myindex[index_num].blk);
                    tc_size++;
                    // oldtempy = tempy;
                } else {
                    if (tempx > oldtempx) {
                        //˵��Ҫ�µ���������
                        index_num++;
                        myindex[index_num].a = tempx;
                        myindex[index_num].blk = i + addr - 1;
                        oldtempx = tempx;
                        SETBLKTEM(temp_blk, tc_size, myindex[index_num].a,
                                  myindex[index_num].blk);
                        tc_size++;
                    }
                }
                //ÿд7�����ݣ���Ӧ����������д��
                if (tc_size == 7) {
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                    writeBlockToDisk(temp_blk, outaddr, buf);
                    freeBlockInBuffer(temp_blk, buf);
                    temp_blk = getNewBlockInBuffer(buf);
                    outaddr++;
                    tc_size = 0;
                }
            }
            freeBlockInBuffer(blk, buf);
        }
        //���16���鶼�������ˣ�����ʣ�ģ�����һ��������ֵŪ��0������
        if (tc_size != 0) {
            if (tc_size < 7) {
                SETBLKTEM(temp_blk, tc_size, 0, 0);
            }
            SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

            writeBlockToDisk(temp_blk, outaddr, buf);
            freeBlockInBuffer(temp_blk, buf);
        }

    } else if (len == 224) {
        //����Ƕ�s�Ľ�������
        for (i = 1; i < 33; i++) {
            blk = readBlockFromDisk(i - 1 + addr, buf);
            printf("�������ݿ�%d\n", i - 1 + addr);
            for (j = 0; j < 7; j++) {
                GETBLKTEM(blk, j, &tempx, &tempy);
                // printf("%d, %d ",tempx,tempy);
                tem_array2[(i - 1) * 7 + j][0] = tempx;
                tem_array2[(i - 1) * 7 + j][1] = tempy;
                //������Ӧ�ñ߶����ݱ߽�������
                //����ǵ�һ��
                if (i == 1 && j == 0) {
                    myindex[index_num].a = tempx;
                    myindex[index_num].blk = i + addr - 1;
                    oldtempx = tempx;
                    SETBLKTEM(temp_blk, tc_size, myindex[index_num].a,
                              myindex[index_num].blk);
                    tc_size++;
                    // oldtempy = tempy;
                } else {
                    if (tempx > oldtempx) {
                        //˵��Ҫ�µ���������
                        index_num++;
                        myindex[index_num].a = tempx;
                        myindex[index_num].blk = i + addr - 1;
                        oldtempx = tempx;
                        SETBLKTEM(temp_blk, tc_size, myindex[index_num].a,
                                  myindex[index_num].blk);
                        tc_size++;
                    }
                }
                //ÿд7�����ݣ���Ӧ����������д��
                if (tc_size == 7) {
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                    writeBlockToDisk(temp_blk, outaddr, buf);
                    freeBlockInBuffer(temp_blk, buf);
                    temp_blk = getNewBlockInBuffer(buf);
                    outaddr++;
                    tc_size = 0;
                }
            }
            freeBlockInBuffer(blk, buf);
        }
        //���32���鶼�������ˣ�����ʣ�ģ�����һ��������ֵŪ��0������
        if (tc_size != 0) {
            if (tc_size < 7) {
                SETBLKTEM(temp_blk, tc_size, 0, 0);
            }
            SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

            writeBlockToDisk(temp_blk, outaddr, buf);
            freeBlockInBuffer(temp_blk, buf);
        }
    }
}

void MULSORT(int mul_num, int total, int begin, int outputnum, Buffer *buf) {
    unsigned char *blk1, *blk2, *blk3, *blk4, *blk5, *blk6, *temp_blk1;
    int tc_size = 0;
    int tc_iosize = 0;
    int i = 0;
    int line_count;
    if (mul_num == 3) {
        printf("3Lu\n");

        //��200��ʼ��R
        line_count = 200;
        //��ʾ�����ڼ�λ��
        int i1 = 0;
        int i2 = 0;
        int i3 = 0;
        //��ʾ�Ƿ�����������ˣ�0��û�ж���
        int i11 = 0;
        int i22 = 0;
        int i33 = 0;
        //��ʾ�����ڼ�����
        int k1 = 0;
        int k2 = 0;
        int k3 = 0;

        int minimal_temp = 0;
        int tempx1, tempy1, tempx2, tempy2, tempx3, tempy3;
        blk1 = readBlockFromDisk(begin + k1, buf);
        blk2 = readBlockFromDisk(begin + 6 + k2, buf);
        blk3 = readBlockFromDisk(begin + 12 + k3, buf);
        temp_blk1 = getNewBlockInBuffer(buf);
        printf("��ʼ�Ƚ���\n");
        while (i < total * 7) {
            GETBLKTEM_MUL(blk1, i1, &tempx1, &tempy1, i11);
            GETBLKTEM_MUL(blk2, i2, &tempx2, &tempy2, i22);
            GETBLKTEM_MUL(blk3, i3, &tempx3, &tempy3, i33);
            minimal_temp =
                TRICOM(tempx1, tempx2, tempx3, tempy1, tempy2, tempy3);
            if (minimal_temp == 1) {
                printf("�ӵ�һ��ѡ\n");
                i1++;
                SETBLKTEM(temp_blk1, tc_size, tempx1, tempy1);
            } else if (minimal_temp == 2) {
                printf("�ӵ�2��ѡ\n");
                i2++;
                SETBLKTEM(temp_blk1, tc_size, tempx2, tempy2);
            } else if (minimal_temp == 3) {
                printf("�ӵ�3��ѡ\n");
                i3++;
                SETBLKTEM(temp_blk1, tc_size, tempx3, tempy3);
            }
            tc_size++;
            //������������
            if (tc_size == 7) {
                SETBLKTEM(temp_blk1, 7, line_count + 1, 0);

                writeBlockToDisk(temp_blk1, line_count, buf);
                line_count++;
                tc_size = 0;
                tc_iosize++;
            }
            //Ȼ�������3�����е�ĳһ��������
            if (i1 == 7) {
                i1 = 0;
                k1++;
                freeBlockInBuffer(blk1, buf);
                if (k1 < 6) {
                    blk1 = readBlockFromDisk(begin + k1, buf);
                }
                //�������6�飬˵����������Ѿ�������
                if (k1 == 6) {
                    i11 = 1;
                    //��ʱBlk1��˭����,ֻҪ��Խ��
                    blk1 = readBlockFromDisk(begin, buf);
                }
            }
            if (i2 == 7) {
                i2 = 0;
                k2++;
                freeBlockInBuffer(blk2, buf);
                if (k2 < 6) {
                    blk2 = readBlockFromDisk(begin + 6 + k2, buf);
                }  //�������6�飬˵����������Ѿ�������
                if (k2 == 6) {
                    i22 = 2;
                    blk2 = readBlockFromDisk(begin, buf);
                }
            }
            if (i3 == 7) {
                i3 = 0;
                k3++;
                freeBlockInBuffer(blk3, buf);
                if (k3 < 4) {
                    blk3 = readBlockFromDisk(begin + 12 + k3, buf);
                }
                //�������4�飬˵����������Ѿ�������
                if (k3 == 4) {
                    i33 = 3;
                    blk3 = readBlockFromDisk(begin, buf);
                }
            }
            i++;
        }

    } else if (mul_num == 6) {
        printf("6Lu\n");
        //��300��ʼ��S
        line_count = 400;
        //��ʾ�����ڼ�λ��
        int i1 = 0;
        int i2 = 0;
        int i3 = 0;
        int i4 = 0;
        int i5 = 0;
        int i6 = 0;
        //��ʾ�Ƿ�����������ˣ�0��û�ж���
        int i11 = 0;
        int i22 = 0;
        int i33 = 0;
        int i44 = 0;
        int i55 = 0;
        int i66 = 0;
        //��ʾ�����ڼ�����
        int k1 = 0;
        int k2 = 0;
        int k3 = 0;
        int k4 = 0;
        int k5 = 0;
        int k6 = 0;
        //����������Ƚϵ���ʱ����
        int x1, y1, x2, y2 = 0;
        int final = -1;

        int minimal_temp = 0;
        int minimal_temp2 = 0;
        int tempx1, tempy1, tempx2, tempy2, tempx3, tempy3;
        int tempx4, tempy4, tempx5, tempy5, tempx6, tempy6;
        blk1 = readBlockFromDisk(begin + k1, buf);
        blk2 = readBlockFromDisk(begin + 6 + k2, buf);
        blk3 = readBlockFromDisk(begin + 12 + k3, buf);
        blk4 = readBlockFromDisk(begin + 18 + k4, buf);
        blk5 = readBlockFromDisk(begin + 24 + k5, buf);
        blk6 = readBlockFromDisk(begin + 30 + k6, buf);
        temp_blk1 = getNewBlockInBuffer(buf);
        printf("��ʼ�Ƚ���\n");
        while (i < total * 7) {
            //��3��3���ıȽ�
            GETBLKTEM_MUL(blk1, i1, &tempx1, &tempy1, i11);
            GETBLKTEM_MUL(blk2, i2, &tempx2, &tempy2, i22);
            GETBLKTEM_MUL(blk3, i3, &tempx3, &tempy3, i33);
            minimal_temp = TRICOM_RES(tempx1, tempx2, tempx3, tempy1, tempy2,
                                      tempy3, &x1, &y1);
            GETBLKTEM_MUL(blk4, i4, &tempx4, &tempy4, i44);
            GETBLKTEM_MUL(blk5, i5, &tempx5, &tempy5, i55);
            GETBLKTEM_MUL(blk6, i6, &tempx6, &tempy6, i66);
            minimal_temp2 = TRICOM_RES(tempx4, tempx5, tempx6, tempy4, tempy5,
                                       tempy6, &x2, &y2);
            if (x1 < x2) {
                final = minimal_temp;
            } else if (x1 > x2) {
                final = minimal_temp2 + 3;
            } else if (x1 == x2) {
                if (y1 < y2) {
                    final = minimal_temp;
                } else {
                    final = minimal_temp2 + 3;
                }
            }

            if (final == 1) {
                printf("�ӵ�1��ѡ\n");
                i1++;
                SETBLKTEM(temp_blk1, tc_size, tempx1, tempy1);
            } else if (final == 2) {
                printf("�ӵ�2��ѡ\n");
                i2++;
                SETBLKTEM(temp_blk1, tc_size, tempx2, tempy2);
            } else if (final == 3) {
                printf("�ӵ�3��ѡ\n");
                i3++;
                SETBLKTEM(temp_blk1, tc_size, tempx3, tempy3);
            } else if (final == 4) {
                printf("�ӵ�4��ѡ\n");
                i4++;
                SETBLKTEM(temp_blk1, tc_size, tempx4, tempy4);
            } else if (final == 5) {
                printf("�ӵ�5��ѡ\n");
                i5++;
                SETBLKTEM(temp_blk1, tc_size, tempx5, tempy5);
            } else if (final == 6) {
                printf("�ӵ�6��ѡ\n");
                i6++;
                SETBLKTEM(temp_blk1, tc_size, tempx6, tempy6);
            }
            tc_size++;
            //������������
            if (tc_size == 7) {
                SETBLKTEM(temp_blk1, 7, line_count + 1, 0);

                writeBlockToDisk(temp_blk1, line_count, buf);
                line_count++;
                tc_size = 0;
                tc_iosize++;
            }
            //Ȼ�������3�����е�ĳһ��������
            if (i1 == 7) {
                i1 = 0;
                k1++;
                freeBlockInBuffer(blk1, buf);
                if (k1 < 6) {
                    blk1 = readBlockFromDisk(begin + k1, buf);
                }
                //�������6�飬˵����������Ѿ�������
                if (k1 == 6) {
                    i11 = 1;
                    //��ʱBlk1��˭����,ֻҪ��Խ��
                    blk1 = readBlockFromDisk(begin, buf);
                }
            }
            if (i2 == 7) {
                i2 = 0;
                k2++;
                freeBlockInBuffer(blk2, buf);
                if (k2 < 6) {
                    blk2 = readBlockFromDisk(begin + 6 + k2, buf);
                }  //�������6�飬˵����������Ѿ�������
                if (k2 == 6) {
                    i22 = 2;
                    blk2 = readBlockFromDisk(begin, buf);
                }
            }
            if (i3 == 7) {
                i3 = 0;
                k3++;
                freeBlockInBuffer(blk3, buf);
                if (k3 < 6) {
                    blk3 = readBlockFromDisk(begin + 12 + k3, buf);
                }
                //�������4�飬˵����������Ѿ�������
                if (k3 == 6) {
                    i33 = 3;
                    blk3 = readBlockFromDisk(begin, buf);
                }
            }
            if (i4 == 7) {
                i4 = 0;
                k4++;
                freeBlockInBuffer(blk4, buf);
                if (k4 < 6) {
                    blk4 = readBlockFromDisk(begin + 18 + k4, buf);
                }
                //�������6�飬˵����������Ѿ�������
                if (k4 == 6) {
                    i44 = 4;
                    //��ʱBlk1��˭����,ֻҪ��Խ��
                    blk4 = readBlockFromDisk(begin, buf);
                }
            }
            if (i5 == 7) {
                i5 = 0;
                k5++;
                freeBlockInBuffer(blk5, buf);
                if (k5 < 6) {
                    blk5 = readBlockFromDisk(begin + 24 + k5, buf);
                }  //�������6�飬˵����������Ѿ�������
                if (k5 == 6) {
                    i55 = 5;
                    blk5 = readBlockFromDisk(begin, buf);
                }
            }
            if (i6 == 7) {
                i6 = 0;
                k6++;
                freeBlockInBuffer(blk6, buf);
                if (k6 < 2) {
                    blk6 = readBlockFromDisk(begin + 30 + k6, buf);
                }
                //�������4�飬˵����������Ѿ�������
                if (k6 == 2) {
                    i66 = 6;
                    blk6 = readBlockFromDisk(begin, buf);
                }
            }
            i++;
        }
    }
}

// resultx,resulty������Сֵ
int TRICOM_RES(int a, int b, int c, int a1, int b1, int c1, int *resultx,
               int *resulty) {
    if (a < b && a < c) {
        *resultx = a;
        *resulty = a1;
        return 1;
    } else if (b < a && b < c) {
        *resultx = b;
        *resulty = b1;
        return 2;
    } else if (c < a && c < b) {
        *resultx = c;
        *resulty = c1;
        return 3;
    } else if (a == b && a != c) {
        if (a1 < b1) {
            *resultx = a;
            *resulty = a1;
            return 1;
        } else if (a1 > b1) {
            *resultx = b;
            *resulty = b1;
            return 2;
        } else {
            //������de���
            *resultx = 0;
            *resulty = 0;
            return 0;
        }

    } else if (a == c && a != b) {
        if (a1 < c1) {
            *resultx = a;
            *resulty = a1;
            return 1;
        } else if (a1 > c1) {
            *resultx = c;
            *resulty = c1;
            return 3;
        } else {
            *resultx = 0;
            *resulty = 0;
            return 0;
        }
    } else if (b == c && a != c) {
        if (b1 < c1) {
            *resultx = b;
            *resulty = b1;
            return 2;
        } else if (b1 > c1) {
            *resultx = c;
            *resulty = c1;
            return 3;
        } else {
            *resultx = 0;
            *resulty = 0;
            return 0;
        }
    } else if (a == b && b == c) {
        if (a1 < b1 && a1 < c1) {
            *resultx = a;
            *resulty = a1;
            return 1;
        } else if (b1 < a1 && b1 < c1) {
            *resultx = b;
            *resulty = b1;
            return 2;
        } else if (c1 < a1 && c1 < b1) {
            *resultx = c;
            *resulty = c1;
            return 3;
        } else {
            *resultx = 0;
            *resulty = 0;
            return 0;
        }
    }
}

//�����Ƚϴ�С�����￼�ǵڶ�ά
// 1/2/3��ʾ˭��С��0��ʾ������ϵ�г�������ȫ��ͬ��Ԫ��
int TRICOM(int a, int b, int c, int a1, int b1, int c1) {
    if (a < b && a < c) {
        return 1;
    } else if (b < a && b < c) {
        return 2;
    } else if (c < a && c < b) {
        return 3;
    } else if (a == b && a != c) {
        if (a1 < b1) {
            return 1;
        } else if (a1 > b1) {
            return 2;
        } else {
            return 0;
        }

    } else if (a == c && a != b) {
        if (a1 < c1) {
            return 1;
        } else if (a1 > c1) {
            return 3;
        } else {
            return 0;
        }
    } else if (b == c && a != c) {
        if (b1 < c1) {
            return 2;
        } else if (b1 > c1) {
            return 3;
        } else {
            return 0;
        }
    } else if (a == b && b == c) {
        if (a1 < b1 && a1 < c1) {
            return 1;
        } else if (b1 < a1 && b1 < c1) {
            return 2;
        } else if (c1 < a1 && c1 < b1) {
            return 3;
        } else {
            return 0;
        }
    }
}

//��ӡ����blk������addr
void PRINTBLK(unsigned char *blk) {
    int X = -1;
    int Y = -1;
    int addr = -1;
    int i;
    char str[5];
    printf("block :\n");
    for (i = 0; i < 7; i++) {
        for (int k = 0; k < 4; k++) {
            str[k] = *(blk + i * 8 + k);
        }
        X = atoi(str);
        for (int k = 0; k < 4; k++) {
            str[k] = *(blk + i * 8 + 4 + k);
        }
        Y = atoi(str);
        printf("(%d, %d) ", X, Y);
    }
    for (int k = 0; k < 4; k++) {
        str[k] = *(blk + i * 8 + k);
    }
    addr = atoi(str);
    printf("\nnext address = %d \n", addr);
}

// getblktem���ĳ��blkָ����k����x��y����(�ӵ�0����ʼ�����Ϊ6)

void GETBLKTEM(unsigned char *blk, int k, int *x, int *y) {
    int i;
    char str[5];

    i = k;
    for (int k = 0; k < 4; k++) {
        str[k] = *(blk + i * 8 + k);
    }
    *x = atoi(str);
    for (int k = 0; k < 4; k++) {
        str[k] = *(blk + i * 8 + 4 + k);
    }
    *y = atoi(str);
    // printf("(%d, %d) ", *x, *y);
}
//��blkָ����k������Ϊx��y
void SETBLKTEM(unsigned char *blk, int k, int x, int y) {
    char str[5];
    int i;
    itoa(x, str, 10);
    printf("%s\t", str);
    i = k;
    for (int k = 0; k < 4; k++) {
        *(blk + i * 8 + k) = str[k];
    }
    // y=0��ʾ�����һ����̿�
    if (y != 0) {
        itoa(y, str, 10);
        printf("%s\n", str);
        for (int k = 0; k < 4; k++) {
            *(blk + i * 8 + 4 + k) = str[k];
        }
    }
}

void GETBLKTEM_MUL(unsigned char *blk, int k, int *x, int *y, int valid) {
    // valid=1������Ч

    int i;
    char str[5];

    i = k;
    for (int k = 0; k < 4; k++) {
        str[k] = *(blk + i * 8 + k);
    }
    *x = atoi(str);
    for (int k = 0; k < 4; k++) {
        str[k] = *(blk + i * 8 + 4 + k);
    }
    *y = atoi(str);
    if (valid != 0) {
        *x = MAX + valid;
        *y = MAX + valid;
    }
}

void SORTTEM(int tem[][2], int row) {
    int i = 0;
    int j = 0;
    int temp1, temp2;
    for (i = 0; i < row - 1; i++) {
        /* code */
        for (j = i + 1; j < row; j++) {
            if (tem[i][0] > tem[j][0]) {
                temp1 = tem[i][0];
                tem[i][0] = tem[j][0];
                tem[j][0] = temp1;
                temp2 = tem[i][1];
                tem[i][1] = tem[j][1];
                tem[j][1] = temp2;
            } else if (tem[i][0] == tem[j][0]) {
                if (tem[i][1] > tem[j][1]) {
                    temp1 = tem[i][0];
                    tem[i][0] = tem[j][0];
                    tem[j][0] = temp1;
                    temp2 = tem[i][1];
                    tem[i][1] = tem[j][1];
                    tem[j][1] = temp2;
                }
            }
        }
    }
}

int main() {
    Buffer buf;         /* A buffer */
    unsigned char *blk; /* A pointer to a block */
    int i = 0;

    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf)) {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    printf(
        "\t1 ���Բ���ok  \t2 ���׶ζ�·�鲢�����㷨R \t3 "
        "���׶ζ�·�鲢�����㷨S\t 4�������� \t5 ������R \t 6������S "
        "7  ��ϵͶӰ�㷨\t "
        "8���������ɢ�е�����ɨ���㷨��ʵ������һ�ּ��ϲ����㷨");
    int choice = 0;
    int j, a, b, c, d;
    int line_count = 60;
    unsigned int *disk_blk;
    unsigned char *temp_blk;
    // uint 32bits
    while (choice != -1) {
        // scanf("%d", &choice);
        choice = 10;
        switch (choice) {
            case 0: {
                //��������
                //��ӡ����Ԫ��
                for (int j = 400; j < 432; j++) {
                    if ((blk = readBlockFromDisk(j, &buf)) == NULL) {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                    PRINTBLK(blk);
                    freeBlockInBuffer(blk, &buf);
                }
                // printf("����������215��");
                // blk = readBlockFromDisk(215, &buf);
                // PRINTBLK(blk);
                // freeBlockInBuffer(blk, &buf);
                // int tempx, tempy;
                // for (j = 0; j < 7; j++) {
                //     GETBLKTEM(blk, j, &tempx, &tempy);
                //     printf("%d,%d", tempx, tempy);
                // }

                choice = -1;
                break;
            }
            case 1:  //ѡ��R.A=30��S.C=23��Ԫ��
            {
                int tempx, tempy;
                // GETBLKTEM(blk, 1, &tempx, &tempy);
                printf("��ʼѡ��R.A=30��\n");
                //��һԪ�鳬��,������Ϊ6
                int tc_size = 0;
                int tc_iosize = 0;
                temp_blk = getNewBlockInBuffer(&buf);

                for (i = 1; i < 17; i++) {
                    blk = readBlockFromDisk(i, &buf);
                    printf("�������ݿ�%d\n", i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 30) {
                            //�������﷢����һ��Ԫ���Ժ�Ӧ���ȿ���һ���µĻ������飬д�����Ԫ�飬������˵Ļ�д����̺���������
                            SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                            tc_size++;
                        }
                        if (tc_size == 7) {
                            writeBlockToDisk(temp_blk, line_count, &buf);
                            line_count++;
                            tc_size = 0;
                            tc_iosize++;
                        }
                    }
                    // printf("buf->numFreeBlk is %d\n",buf.numFreeBlk);
                    freeBlockInBuffer(blk, &buf);
                    // printf("buf->numFreeBlk is %d\n",buf.numFreeBlk);
                }
                //���16�������Ժ���ʣ�µ�
                if (tc_size != 0) {
                    writeBlockToDisk(temp_blk, line_count, &buf);
                }
                printf("�ҵ�����ѡ��������Ԫ��һ��%d��\n",
                       tc_iosize * 7 + tc_size);
                printf("�ܹ�io%d��\n", buf.numIO);

                line_count += 10;

                printf("\n");

                printf("��ʼѡ��S.C=23��\n");
                //��һԪ�鳬��,������Ϊ6
                tc_size = 0;
                tc_iosize = 0;
                temp_blk = getNewBlockInBuffer(&buf);

                for (i = 17; i < 49; i++) {
                    blk = readBlockFromDisk(i, &buf);
                    printf("�������ݿ�%d\n", i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 23) {
                            //�������﷢����һ��Ԫ���Ժ�Ӧ���ȿ���һ���µĻ������飬д�����Ԫ�飬������˵Ļ�д����̺���������
                            SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                            tc_size++;
                        }
                        if (tc_size == 7) {
                            writeBlockToDisk(temp_blk, line_count, &buf);
                            line_count++;
                            tc_size = 0;
                            tc_iosize++;
                        }
                    }
                    // printf("buf->numFreeBlk is %d\n",buf.numFreeBlk);
                    freeBlockInBuffer(blk, &buf);
                    // printf("buf->numFreeBlk is %d\n",buf.numFreeBlk);
                }
                //���16�������Ժ���ʣ�µ�
                if (tc_size != 0) {
                    writeBlockToDisk(temp_blk, line_count, &buf);
                }
                printf("�ҵ�����ѡ��������Ԫ��һ��%d��\n",
                       tc_iosize * 7 + tc_size);
                printf("�ܹ�io%d��\n", buf.numIO);

                line_count += 10;
                choice = -1;
                printf("\n");
                break;
            }
            case 2: {
                //ʵ�����׶ζ�·�鲢�����㷨��TPMMS���������ڴ滺��������ϵR��S�ֱ����򣬲�������
                //��Ľ������ڴ����ϡ� ����R,�ܹ���16�飬����6��6���װ
                // 100.blk��ʼ
                int tempx, tempy;
                int tem_array1[42][2] = {0};
                int tc_size = 0;
                int tc_iosize = 0;
                line_count = 100;
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i, &buf);
                    printf("�������ݿ�%d\n", i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                // for (i = 0; i < 42; i++) {
                //     printf("֮ǰtem_array1 is %d,%d\n", tem_array1[i][0],
                //            tem_array1[i][1]);
                //            }
                SORTTEM(tem_array1, 42);
                //����һ��
                temp_blk = getNewBlockInBuffer(&buf);

                // for (i = 0; i < 42; i++) {
                //     printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                //            tem_array1[i][1]);
                // }
                //��Ū����д��
                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);
                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                // printf("��������һ�������%d��",tc_iosize);
                //�����û����
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // line_count++;

                //Ȼ����7-13��
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 6, &buf);
                    printf("�������ݿ�%d\n", i + 6);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                SORTTEM(tem_array1, 42);

                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);

                    //��Ū����д��

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                //�����û����,Ŷ���Բ�������û����
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // line_count++;

                // Ȼ����13-16��
                for (i = 1; i < 5; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 12, &buf);
                    printf("�������ݿ�%d\n", i + 12);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                SORTTEM(tem_array1, 28);

                for (i = 0; i < 28; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);

                    //��Ū����д��

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                //�����û����
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // ����Ժ��ͷſռ�
                freeBlockInBuffer(temp_blk, &buf);

                //�������Ƕ�·�鲢����
                printf("�������Ƕ�·�鲢����\n");
                MULSORT(3, 16, 100, 200, &buf);

                printf("\n");
                choice = -1;
                break;
            }
            case 3: {
                //ʵ�����׶ζ�·�鲢�����㷨��TPMMS���������ڴ滺��������ϵR��S�ֱ����򣬲�������
                // 300.blk��ʼ
                printf("\n��s��������\n");

                int tempx, tempy;
                int tem_array1[42][2] = {0};
                int tc_size = 0;
                int tc_iosize = 0;
                line_count = 300;
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 16, &buf);
                    printf("�������ݿ�%d\n", i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                // for (i = 0; i < 42; i++) {
                //     printf("֮ǰtem_array1 is %d,%d\n", tem_array1[i][0],
                //            tem_array1[i][1]);
                //            }
                SORTTEM(tem_array1, 42);
                //����һ��
                temp_blk = getNewBlockInBuffer(&buf);

                // for (i = 0; i < 42; i++) {
                //     printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                //            tem_array1[i][1]);
                // }
                //��Ū����д��
                printf("\n");

                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);
                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        freeBlockInBuffer(temp_blk, &buf);
                        temp_blk = getNewBlockInBuffer(&buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                // printf("��������һ�������%d��",tc_iosize);
                //�����û����
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // line_count++;

                //Ȼ����7-13��
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 6 + 16, &buf);
                    printf("�������ݿ�%d\n", i + 6);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                SORTTEM(tem_array1, 42);
                printf("\n");
                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);

                    //��Ū����д��

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }

                // Ȼ����13-18��
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 12 + 16, &buf);
                    printf("�������ݿ�%d\n", i + 12);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                SORTTEM(tem_array1, 42);

                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);

                    //��Ū����д��

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                // 19-24
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 18 + 16, &buf);
                    printf("�������ݿ�%d\n", i + 18);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                SORTTEM(tem_array1, 42);

                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);

                    //��Ū����д��

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                // 25-30
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 24 + 16, &buf);
                    printf("�������ݿ�%d\n", i + 24);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                SORTTEM(tem_array1, 42);

                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);

                    //��Ū����д��

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                // 31-32
                for (i = 1; i < 3; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 30 + 16, &buf);
                    printf("�������ݿ�%d\n", i + 30);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                SORTTEM(tem_array1, 14);

                for (i = 0; i < 14; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);

                    //��Ū����д��

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //������˵Ļ�
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }

                //�����û����
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // ����Ժ��ͷſռ�
                freeBlockInBuffer(temp_blk, &buf);

                //�������Ƕ�·�鲢����
                printf("�������Ƕ�·�鲢����\n");
                MULSORT(6, 32, 300, 400, &buf);

                printf("\n");
                choice = -1;
                break;
            }

            case 4: {
                //ʵ�ֻ��������Ĺ�ϵѡ���㷨�����ã�2���е�������Ϊ��ϵR��S�ֱ��������ļ���
                //���������ļ�ѡ��R.A=30��S.C=23��Ԫ�飬����ѡ��������ڴ����ϡ���¼IO��д������
                //�루1���еĽ���Աȡ�
                // 600��ʼ��r��������
                index *indexR = CreateIndex(&buf, 200, 600, 112);
                index *indexS = CreateIndex(&buf, 400, 650, 224);

                choice = -1;
                break;
            }
            case 5: {
                //���������ļ�ѡ��R.A=30��S.C=23��Ԫ�飬����ѡ��������ڴ����ϡ���¼IO��д����
                //������Ҫ�������飬�ҵ�����ֵΪ30�Ŀ��

                int tempx, tempy;
                // GETBLKTEM(blk, 1, &tempx, &tempy);
                printf("��ʼ��������ѡ��R.A=30��\n");
                //��һԪ�鳬��,������Ϊ6
                int tc_size = 0;
                int tc_iosize = 0;
                int tar_addr1, tar_addr2;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag = 0;  // Moren û�ҵ�
                // r��600-605
                i = 0;
                j = 0;
                while (flag != 1 && i < 5) {
                    blk = readBlockFromDisk(600 + i, &buf);
                    printf("����������%d\n", 600 + i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 30) {
                            //�������﷢����һ��Ԫ���Ժ�Ӧ�÷������Ӧ�������
                            tar_addr1 = tempy;
                            printf("�����ҵ���һ������飬�����ʼΪ%d",
                                   tar_addr1);
                            flag = 1;
                            //�����������ǵ��ļ���˳�����еģ�ֻ��Ҫ�ӿ�ʼ���Ǹ��鿪ʼ����ֱ������30Ϊֹ�Ϳ�����
                        }
                        if (flag == 1) break;
                    }
                    freeBlockInBuffer(blk, &buf);
                    i++;
                }
                //��tar_addr1��ʼ���ʣ�д��700��ʼ�Ŀ�����
                int outputaddr = 700;
                int flag1 = 0;
                i = 0;
                while (flag1 != 1) {
                    blk = readBlockFromDisk(tar_addr1 + i, &buf);
                    printf("�����ļ����ݿ�%d\n", tar_addr1 + i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 30) {
                            //������30�ģ�д��700
                            SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                            tc_size++;

                            if (tc_size == 7) {
                                writeBlockToDisk(temp_blk, outputaddr, &buf);
                                outputaddr++;
                                tc_size = 0;
                                tc_iosize++;
                            }
                        } else if (tempx > 30) {
                            flag1 = 1;
                        }
                        if (flag1 == 1) break;
                    }
                    freeBlockInBuffer(blk, &buf);
                    i++;
                }

                if (tc_size != 0) {
                    SETBLKTEM(temp_blk, 7, outputaddr + 1, 0);

                    writeBlockToDisk(temp_blk, outputaddr, &buf);
                }
                freeBlockInBuffer(temp_blk, &buf);

                printf("�ҵ�����ѡ��������Ԫ��һ��%d��\n",
                       tc_iosize * 7 + tc_size);
                printf("�ܹ�io%d��\n", buf.numIO);

                choice = -1;
                break;
            }

            case 6: {
                int tempx, tempy;
                printf("��ʼ��������ѡ��S.C=23��\n");
                //��һԪ�鳬��,������Ϊ6
                int tc_size = 0;
                int tc_iosize = 0;
                int tar_addr1, tar_addr2;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag = 0;  // Moren û�ҵ�
                // r��600-605
                // s��650-657
                i = 0;
                j = 0;
                while (flag != 1 && i < 7) {
                    blk = readBlockFromDisk(650 + i, &buf);
                    printf("����������%d\n", 650 + i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 23) {
                            //�������﷢����һ��Ԫ���Ժ�Ӧ�÷������Ӧ�������
                            tar_addr1 = tempy;
                            printf("�����ҵ���һ������飬�����ʼΪ%d",
                                   tar_addr1);
                            flag = 1;
                            //�����������ǵ��ļ���˳�����еģ�ֻ��Ҫ�ӿ�ʼ���Ǹ��鿪ʼ����ֱ������30Ϊֹ�Ϳ�����
                        }
                        if (flag == 1) break;
                    }
                    freeBlockInBuffer(blk, &buf);
                    i++;
                }
                //��tar_addr1��ʼ���ʣ�д��700��ʼ�Ŀ�����
                int outputaddr = 710;
                int flag1 = 0;
                i = 0;
                while (flag1 != 1) {
                    blk = readBlockFromDisk(tar_addr1 + i, &buf);
                    printf("�����ļ����ݿ�%d\n", tar_addr1 + i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 23) {
                            //������23�ģ�д��710
                            SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                            tc_size++;

                            if (tc_size == 7) {
                                writeBlockToDisk(temp_blk, outputaddr, &buf);
                                outputaddr++;
                                tc_size = 0;
                                tc_iosize++;
                            }
                        } else if (tempx > 23) {
                            flag1 = 1;
                        }
                        if (flag1 == 1) break;
                    }
                    freeBlockInBuffer(blk, &buf);
                    i++;
                }

                if (tc_size != 0) {
                    SETBLKTEM(temp_blk, 7, outputaddr + 1, 0);
                    writeBlockToDisk(temp_blk, outputaddr, &buf);
                }
                freeBlockInBuffer(temp_blk, &buf);

                printf("�ҵ�����ѡ��������Ԫ��һ��%d��\n",
                       tc_iosize * 7 + tc_size);
                printf("�ܹ�io%d��\n", buf.numIO);

                choice = -1;
                break;
            }
            case 7: {
                //ʵ�ֹ�ϵͶӰ�㷨���Թ�ϵR�ϵ�A���ԣ������룩����ͶӰ��ȥ�أ����������ڴ�����
                //����ֱ���������Ľ��200-215
                //����720��
                int tempx, tempy, oldtempx;
                int outaddr = 720;
                int tc_size = 0;
                int tc_iosize = 0;
                printf("�������\n");
                temp_blk = getNewBlockInBuffer(&buf);
                for (i = 1; i < 17; i++) {
                    blk = readBlockFromDisk(i - 1 + 200, &buf);
                    printf("�������ݿ�%d\n", i - 1 + 200);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);

                        if (i == 1 && j == 0) {
                            oldtempx = tempx;
                            //����y��������99
                            SETBLKTEM(temp_blk, tc_size, tempx, 99);
                            tc_size++;
                        } else {
                            if (tempx > oldtempx) {
                                oldtempx = tempx;
                                SETBLKTEM(temp_blk, tc_size, tempx, 99);

                                tc_size++;
                            }
                        }
                        //ÿд7�����ݣ���Ӧ����������д��
                        if (tc_size == 7) {
                            SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                            writeBlockToDisk(temp_blk, outaddr, &buf);
                            freeBlockInBuffer(temp_blk, &buf);
                            temp_blk = getNewBlockInBuffer(&buf);
                            outaddr++;
                            tc_iosize++;
                            tc_size = 0;
                        }
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                //���16���鶼�������ˣ�����ʣ�ģ�����һ��������ֵŪ��0������
                if (tc_size != 0) {
                    if (tc_size < 7) {
                        SETBLKTEM(temp_blk, tc_size, 0, 0);
                    }
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

                    writeBlockToDisk(temp_blk, outaddr, &buf);
                    freeBlockInBuffer(temp_blk, &buf);
                }
                printf("����ȥ�ص�����ֵ����%d\n", tc_iosize * 7 + tc_size);
                choice = -1;
                break;
            }
            case 8: {
                //ʵ�ֻ�����������Ӳ����㷨��Sort-Merge-Join�����Թ�ϵR��S����R.A����S.C����ͳ�����Ӵ����������ӽ������ڴ����ϡ�
                int join_num = 0;
                int tempx, tempy, oldtempx;
                int tempx2, tempy2;
                // R.A����S.C 730�����ӵĽ��
                // R��200��ʼ S��400��ʼ
                int is, js;
                int outaddr = 730;
                int tc_size = 0;
                int tc_iosize = 0;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag1 = 0;
                unsigned char *blk_s;
                for (i = 1; i < 17; i++) {
                    blk = readBlockFromDisk(i - 1 + 200, &buf);
                    printf("�������ݿ�%d\n", i - 1 + 200);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        flag1 = 0;

                        //�����ȡ��һ��blk������������S�Ŀ�ʼ����
                        is = 1;
                        while ((is < 33) && (flag1 != 1)) {
                            blk_s = readBlockFromDisk(is - 1 + 400, &buf);
                            for (js = 0; js < 7; js++) {
                                GETBLKTEM(blk_s, js, &tempx2, &tempy2);
                                if (tempx == tempx2) {
                                    //���Ӵ���++
                                    join_num++;
                                    SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                                    tc_size++;
                                    //�п�������
                                    if (tc_size == 7) {
                                        SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                                        writeBlockToDisk(temp_blk, outaddr,
                                                         &buf);
                                        freeBlockInBuffer(temp_blk, &buf);
                                        temp_blk = getNewBlockInBuffer(&buf);
                                        outaddr++;
                                        tc_iosize++;
                                        tc_size = 0;
                                    }
                                    SETBLKTEM(temp_blk, tc_size, tempx2,
                                              tempy2);
                                    tc_size++;
                                    if (tc_size == 7) {
                                        SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                                        writeBlockToDisk(temp_blk, outaddr,
                                                         &buf);
                                        freeBlockInBuffer(temp_blk, &buf);
                                        temp_blk = getNewBlockInBuffer(&buf);
                                        outaddr++;
                                        tc_iosize++;
                                        tc_size = 0;
                                    }
                                } else if (tempx < tempx2) {
                                    flag1 = 1;
                                    break;
                                }
                            }
                            freeBlockInBuffer(blk_s, &buf);  //����ȥ
                            is++;
                        }
                        printf("R�еĿ�R.A= %d���ܹ�����%d��\n", tempx,
                               join_num);
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                //���16���鶼�������ˣ�����ʣ�ģ�����һ��������ֵŪ��0������
                if (tc_size != 0) {
                    if (tc_size < 7) {
                        SETBLKTEM(temp_blk, tc_size, 0, 0);
                    }
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

                    writeBlockToDisk(temp_blk, outaddr, &buf);
                    freeBlockInBuffer(temp_blk, &buf);
                }

                printf("�ܹ�����%d��\n", join_num);
                choice = -1;
                break;
            }

            case 9: {
                //���������ɢ�е�����ɨ���㷨��ʵ������һ�ּ��ϲ����㷨�����������S-R���е�һ�֡����������ڴ����ϣ���ͳ�Ʋ���������������Ԫ�������
                printf("���������󽻼�\n");
                //����ǰ�������Ѿ�����100-115
                /// 300-331�ĵ�һ�����Ž��������ֱ�ӽ���
                //����ֱ�Ӱ�R�������ý���������Ȼ��ֱ���S�Ľ����Ƚϼ���
                int tempx, tempy, oldtempx;
                int tempx2, tempy2;
                int join_num = 0;  //�ཻ���ϸ���
                int is, js;
                int outaddr = 800;
                int tc_size = 0;
                int tc_iosize = 0;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag1 = 0;
                unsigned char *blk_s;
                for (i = 1; i < 17; i++) {
                    blk = readBlockFromDisk(i - 1 + 200, &buf);
                    printf("�������ݿ�%d\n", i - 1 + 200);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        flag1 = 0;

                        //�����ȡ��һ��blk������������S�Ŀ�ʼ����
                        is = 1;
                        while ((is < 33) && (flag1 != 1)) {
                            blk_s = readBlockFromDisk(is - 1 + 400, &buf);
                            for (js = 0; js < 7; js++) {
                                GETBLKTEM(blk_s, js, &tempx2, &tempy2);
                                if ((tempx == tempx2) && (tempy == tempy2)) {
                                    //�ཻ����++
                                    join_num++;
                                    SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                                    tc_size++;
                                    //�п�������
                                    if (tc_size == 7) {
                                        SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                                        writeBlockToDisk(temp_blk, outaddr,
                                                         &buf);
                                        freeBlockInBuffer(temp_blk, &buf);
                                        temp_blk = getNewBlockInBuffer(&buf);
                                        outaddr++;
                                        tc_iosize++;
                                        tc_size = 0;
                                    }

                                } else if (tempx < tempx2) {
                                    flag1 = 1;
                                    break;
                                }
                            }
                            freeBlockInBuffer(blk_s, &buf);  //����ȥ
                            is++;
                        }
                        printf("R�еĿ�R.A= %d���ܹ��ཻ%d��\n", tempx,
                               join_num);
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                //���16���鶼�������ˣ�����ʣ�ģ�����һ��������ֵŪ��0������
                if (tc_size != 0) {
                    if (tc_size < 7) {
                        SETBLKTEM(temp_blk, tc_size, 0, 0);
                    }
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

                    writeBlockToDisk(temp_blk, outaddr, &buf);
                    freeBlockInBuffer(temp_blk, &buf);
                }

                choice = -1;
                break;
            }
            case 10: {
                //���������ɢ�е�����ɨ���㷨��ʵ������һ�ּ��ϲ����㷨�����������S-R���е�һ�֡����������ڴ����ϣ���ͳ�Ʋ���������������Ԫ�������
                printf("���������󲢼�\n");
                //����ǰ�������Ѿ�����100-115
                /// 300-331�ĵ�һ�����Ž��������ֱ�ӽ���
                //����ֱ�Ӱ�R�������ý���������Ȼ��ֱ���S�Ľ����Ƚϼ���
                int tempx, tempy, oldtempx;
                int tempx2, tempy2;
                int join_num = 0;
                int is, js, ir, jr;
                int outaddr = 810;
                int tc_size = 0;
                int tc_iosize = 0;
                int length1 = 112;
                int length2 = 224;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag1 = 0;
                int array1[40][2] = {0};
                int effect_num = 0;
                int k = 0;
                unsigned char *blk_s;
                unsigned char *blk_r;

                //�ȶ�S�ģ���ȥ1���飬����ֱ���ö�����1����Ľ��
                //���ڴ�ʱ������ֻʣ��7�飬����RҪ6��Ķ�
                printf("��ʼд���%d\n", outaddr);
                for (i = 1; i < 33; i++) {
                    blk = readBlockFromDisk(i - 1 + 400, &buf);
                    printf("����S�����ݿ�%d\n", i - 1 + 400);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        flag1 = 0;
                        for (ir = 1; ir < 17; ir++) {
                            blk_r = readBlockFromDisk(ir - 1 + 100, &buf);
                            for (jr = 0; jr < 7; jr++) {
                                GETBLKTEM(blk_r, jr, &tempx2, &tempy2);
                                if ((tempx == tempx2) && (tempy == tempy2)) {
                                    flag1 = 1;
                                    effect_num++;
                                }
                            }
                            freeBlockInBuffer(blk_r, &buf);
                        }
                        if (flag1 != 1) {
                            //֤��SbingR�����������д����
                            SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                            tc_size++;
                            //�п�������
                            if (tc_size == 7) {
                                SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                                writeBlockToDisk(temp_blk, outaddr, &buf);
                                freeBlockInBuffer(temp_blk, &buf);
                                temp_blk = getNewBlockInBuffer(&buf);
                                outaddr++;
                                tc_iosize++;
                                tc_size = 0;
                            }
                        }
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                //Ȼ���RҲд��ȥ�ͺ���
                for (ir = 1; ir < 17; ir++) {
                    blk_r = readBlockFromDisk(ir - 1 + 200, &buf);
                    for (jr = 0; jr < 7; jr++) {
                        GETBLKTEM(blk_r, jr, &tempx2, &tempy2);
                        SETBLKTEM(temp_blk,tc_size,tempx,tempy);
                        tc_size++;
                        if (tc_size == 7) {
                                SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                                writeBlockToDisk(temp_blk, outaddr, &buf);
                                freeBlockInBuffer(temp_blk, &buf);
                                temp_blk = getNewBlockInBuffer(&buf);
                                outaddr++;
                                tc_iosize++;
                                tc_size = 0;
                            }
                    }
                    freeBlockInBuffer(blk_r, &buf);
                }

                if (tc_size != 0) {
                    if (tc_size < 7) {
                        SETBLKTEM(temp_blk, tc_size, 0, 0);
                    }
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

                    writeBlockToDisk(temp_blk, outaddr, &buf);
                    freeBlockInBuffer(temp_blk, &buf);
                }

                printf("�������д����%d - %d,s��r�ܹ�%d��Ԫ��", 810, outaddr,
                       length2 + length1 - effect_num);

                choice = -1;
                break;
            }
            case 11: {
                //���������ɢ�е�����ɨ���㷨��ʵ������һ�ּ��ϲ����㷨�����������S-R���е�һ�֡����������ڴ����ϣ���ͳ�Ʋ���������������Ԫ�������
                printf("����������S-R����\n");
                //����ǰ�������Ѿ�����100-115
                /// 300-331�ĵ�һ�����Ž��������ֱ�ӽ���
                int tempx, tempy, oldtempx;
                int tempx2, tempy2;
                int join_num = 0;
                int is, js, ir, jr;
                int outaddr = 900;
                int tc_size = 0;
                int tc_iosize = 0;
                int length1 = 112;
                int length2 = 224;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag1 = 0;
                int array1[40][2] = {0};
                int effect_num = 0;
                int k = 0;
                unsigned char *blk_s;
                unsigned char *blk_r;

                //�ȶ�S�ģ���ȥ1���飬����ֱ���ö�����1����Ľ��
                //���ڴ�ʱ������ֻʣ��7�飬����RҪ6��Ķ�
                printf("��ʼд���%d\n", outaddr);
                for (i = 1; i < 33; i++) {
                    blk = readBlockFromDisk(i - 1 + 400, &buf);
                    printf("����S�����ݿ�%d\n", i - 1 + 200);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        flag1 = 0;
                        for (ir = 1; ir < 17; ir++) {
                            blk_r = readBlockFromDisk(ir - 1 + 100, &buf);
                            for (jr = 0; jr < 7; jr++) {
                                GETBLKTEM(blk_r, jr, &tempx2, &tempy2);
                                if ((tempx == tempx2) && (tempy == tempy2)) {
                                    flag1 = 1;
                                    effect_num++;
                                }
                            }
                            freeBlockInBuffer(blk_r, &buf);
                        }
                        if (flag1 != 1) {
                            //֤��S-R����û�����
                            SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                            tc_size++;
                            //�п�������
                            if (tc_size == 7) {
                                SETBLKTEM(temp_blk, 7, outaddr + 1, 0);
                                writeBlockToDisk(temp_blk, outaddr, &buf);
                                freeBlockInBuffer(temp_blk, &buf);
                                temp_blk = getNewBlockInBuffer(&buf);
                                outaddr++;
                                tc_iosize++;
                                tc_size = 0;
                            }
                        }
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                if (tc_size != 0) {
                    if (tc_size < 7) {
                        SETBLKTEM(temp_blk, tc_size, 0, 0);
                    }
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

                    writeBlockToDisk(temp_blk, outaddr, &buf);
                    freeBlockInBuffer(temp_blk, &buf);
                }

                printf("�������д����%d - %d,s-r�ܹ�%d��Ԫ��", 900, outaddr,
                       length2 - effect_num);

                choice = -1;
                break;
            }
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "extmem.h"
#define MAX 1000
typedef struct index  //索引
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
//指定多路排序,begin是Blk开始的号数，outputnum是输出的blk号数，total是总共的块数

//建立索引，addr是关系开始的号数，len是总的关系的个数,outaddr是索引存在块上的号数
index *CreateIndex(Buffer *buf, int addr, int outaddr, int len);

index *CreateIndex(Buffer *buf, int addr, int outaddr, int len) {
    index *myindex = malloc(sizeof(index) * 40);  //建40个索引
    // r的addr是200
    // s的addr是400
    int i, j, k = 0;
    int index_num = 0;
    unsigned char *blk, *temp_blk;
    int tempx, tempy;
    int oldtempx, oldtempy;
    int line_count;
    int blk_num = len / 7;  // R就是16，S是32
    //先要把关系里面的东西读出来
    int tem_array1[112][2] = {0};
    int tem_array2[224][2] = {0};
    int tc_size = 0;
    temp_blk = getNewBlockInBuffer(buf);

    if (len == 112) {
        outaddr = 600;  // 600开始存r索引
    } else {
        outaddr = 650;  // 650开始存s索引
    }
    //单独测试215块
    // blk = readBlockFromDisk(215, buf);
    // GETBLKTEM(blk, 5, &tempx, &tempy);
    // printf("单独测试结果%d, %d ",tempx,tempy);
    // freeBlockInBuffer(blk, buf);
    // tempx = 0;
    // tempy = 0;

    if (len == 112) {
        //如果是对r的建立索引
        for (i = 1; i < 17; i++) {
            blk = readBlockFromDisk(i - 1 + addr, buf);
            printf("读入数据块%d\n", i - 1 + addr);
            for (j = 0; j < 7; j++) {
                GETBLKTEM(blk, j, &tempx, &tempy);
                // printf("%d, %d ",tempx,tempy);
                tem_array1[(i - 1) * 7 + j][0] = tempx;
                tem_array1[(i - 1) * 7 + j][1] = tempy;
                //我这里应该边读数据边建立索引
                //如果是第一个
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
                        //说明要新的索引块了
                        index_num++;
                        myindex[index_num].a = tempx;
                        myindex[index_num].blk = i + addr - 1;
                        oldtempx = tempx;
                        SETBLKTEM(temp_blk, tc_size, myindex[index_num].a,
                                  myindex[index_num].blk);
                        tc_size++;
                    }
                }
                //每写7个数据，就应该往磁盘里写了
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
        //如果16个块都处理完了，还有剩的，把下一个的属性值弄成0来区别
        if (tc_size != 0) {
            if (tc_size < 7) {
                SETBLKTEM(temp_blk, tc_size, 0, 0);
            }
            SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

            writeBlockToDisk(temp_blk, outaddr, buf);
            freeBlockInBuffer(temp_blk, buf);
        }

    } else if (len == 224) {
        //如果是对s的建立索引
        for (i = 1; i < 33; i++) {
            blk = readBlockFromDisk(i - 1 + addr, buf);
            printf("读入数据块%d\n", i - 1 + addr);
            for (j = 0; j < 7; j++) {
                GETBLKTEM(blk, j, &tempx, &tempy);
                // printf("%d, %d ",tempx,tempy);
                tem_array2[(i - 1) * 7 + j][0] = tempx;
                tem_array2[(i - 1) * 7 + j][1] = tempy;
                //我这里应该边读数据边建立索引
                //如果是第一个
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
                        //说明要新的索引块了
                        index_num++;
                        myindex[index_num].a = tempx;
                        myindex[index_num].blk = i + addr - 1;
                        oldtempx = tempx;
                        SETBLKTEM(temp_blk, tc_size, myindex[index_num].a,
                                  myindex[index_num].blk);
                        tc_size++;
                    }
                }
                //每写7个数据，就应该往磁盘里写了
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
        //如果32个块都处理完了，还有剩的，把下一个的属性值弄成0来区别
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

        //从200开始排R
        line_count = 200;
        //显示读到第几位了
        int i1 = 0;
        int i2 = 0;
        int i3 = 0;
        //显示是否整个组读完了，0还没有读完
        int i11 = 0;
        int i22 = 0;
        int i33 = 0;
        //显示读到第几块了
        int k1 = 0;
        int k2 = 0;
        int k3 = 0;

        int minimal_temp = 0;
        int tempx1, tempy1, tempx2, tempy2, tempx3, tempy3;
        blk1 = readBlockFromDisk(begin + k1, buf);
        blk2 = readBlockFromDisk(begin + 6 + k2, buf);
        blk3 = readBlockFromDisk(begin + 12 + k3, buf);
        temp_blk1 = getNewBlockInBuffer(buf);
        printf("开始比较了\n");
        while (i < total * 7) {
            GETBLKTEM_MUL(blk1, i1, &tempx1, &tempy1, i11);
            GETBLKTEM_MUL(blk2, i2, &tempx2, &tempy2, i22);
            GETBLKTEM_MUL(blk3, i3, &tempx3, &tempy3, i33);
            minimal_temp =
                TRICOM(tempx1, tempx2, tempx3, tempy1, tempy2, tempy3);
            if (minimal_temp == 1) {
                printf("从第一组选\n");
                i1++;
                SETBLKTEM(temp_blk1, tc_size, tempx1, tempy1);
            } else if (minimal_temp == 2) {
                printf("从第2组选\n");
                i2++;
                SETBLKTEM(temp_blk1, tc_size, tempx2, tempy2);
            } else if (minimal_temp == 3) {
                printf("从第3组选\n");
                i3++;
                SETBLKTEM(temp_blk1, tc_size, tempx3, tempy3);
            }
            tc_size++;
            //如果缓冲块满了
            if (tc_size == 7) {
                SETBLKTEM(temp_blk1, 7, line_count + 1, 0);

                writeBlockToDisk(temp_blk1, line_count, buf);
                line_count++;
                tc_size = 0;
                tc_iosize++;
            }
            //然后处理如果3个块中的某一个读完了
            if (i1 == 7) {
                i1 = 0;
                k1++;
                freeBlockInBuffer(blk1, buf);
                if (k1 < 6) {
                    blk1 = readBlockFromDisk(begin + k1, buf);
                }
                //如果读了6块，说明这个分组已经读完了
                if (k1 == 6) {
                    i11 = 1;
                    //此时Blk1读谁都行,只要不越界
                    blk1 = readBlockFromDisk(begin, buf);
                }
            }
            if (i2 == 7) {
                i2 = 0;
                k2++;
                freeBlockInBuffer(blk2, buf);
                if (k2 < 6) {
                    blk2 = readBlockFromDisk(begin + 6 + k2, buf);
                }  //如果读了6块，说明这个分组已经读完了
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
                //如果读了4块，说明这个分组已经读完了
                if (k3 == 4) {
                    i33 = 3;
                    blk3 = readBlockFromDisk(begin, buf);
                }
            }
            i++;
        }

    } else if (mul_num == 6) {
        printf("6Lu\n");
        //从300开始排S
        line_count = 400;
        //显示读到第几位了
        int i1 = 0;
        int i2 = 0;
        int i3 = 0;
        int i4 = 0;
        int i5 = 0;
        int i6 = 0;
        //显示是否整个组读完了，0还没有读完
        int i11 = 0;
        int i22 = 0;
        int i33 = 0;
        int i44 = 0;
        int i55 = 0;
        int i66 = 0;
        //显示读到第几块了
        int k1 = 0;
        int k2 = 0;
        int k3 = 0;
        int k4 = 0;
        int k5 = 0;
        int k6 = 0;
        //用来两个组比较的临时变量
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
        printf("开始比较了\n");
        while (i < total * 7) {
            //先3个3个的比较
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
                printf("从第1组选\n");
                i1++;
                SETBLKTEM(temp_blk1, tc_size, tempx1, tempy1);
            } else if (final == 2) {
                printf("从第2组选\n");
                i2++;
                SETBLKTEM(temp_blk1, tc_size, tempx2, tempy2);
            } else if (final == 3) {
                printf("从第3组选\n");
                i3++;
                SETBLKTEM(temp_blk1, tc_size, tempx3, tempy3);
            } else if (final == 4) {
                printf("从第4组选\n");
                i4++;
                SETBLKTEM(temp_blk1, tc_size, tempx4, tempy4);
            } else if (final == 5) {
                printf("从第5组选\n");
                i5++;
                SETBLKTEM(temp_blk1, tc_size, tempx5, tempy5);
            } else if (final == 6) {
                printf("从第6组选\n");
                i6++;
                SETBLKTEM(temp_blk1, tc_size, tempx6, tempy6);
            }
            tc_size++;
            //如果缓冲块满了
            if (tc_size == 7) {
                SETBLKTEM(temp_blk1, 7, line_count + 1, 0);

                writeBlockToDisk(temp_blk1, line_count, buf);
                line_count++;
                tc_size = 0;
                tc_iosize++;
            }
            //然后处理如果3个块中的某一个读完了
            if (i1 == 7) {
                i1 = 0;
                k1++;
                freeBlockInBuffer(blk1, buf);
                if (k1 < 6) {
                    blk1 = readBlockFromDisk(begin + k1, buf);
                }
                //如果读了6块，说明这个分组已经读完了
                if (k1 == 6) {
                    i11 = 1;
                    //此时Blk1读谁都行,只要不越界
                    blk1 = readBlockFromDisk(begin, buf);
                }
            }
            if (i2 == 7) {
                i2 = 0;
                k2++;
                freeBlockInBuffer(blk2, buf);
                if (k2 < 6) {
                    blk2 = readBlockFromDisk(begin + 6 + k2, buf);
                }  //如果读了6块，说明这个分组已经读完了
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
                //如果读了4块，说明这个分组已经读完了
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
                //如果读了6块，说明这个分组已经读完了
                if (k4 == 6) {
                    i44 = 4;
                    //此时Blk1读谁都行,只要不越界
                    blk4 = readBlockFromDisk(begin, buf);
                }
            }
            if (i5 == 7) {
                i5 = 0;
                k5++;
                freeBlockInBuffer(blk5, buf);
                if (k5 < 6) {
                    blk5 = readBlockFromDisk(begin + 24 + k5, buf);
                }  //如果读了6块，说明这个分组已经读完了
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
                //如果读了4块，说明这个分组已经读完了
                if (k6 == 2) {
                    i66 = 6;
                    blk6 = readBlockFromDisk(begin, buf);
                }
            }
            i++;
        }
    }
}

// resultx,resulty返回最小值
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
            //不存在de情况
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

//三数比较大小，这里考虑第二维
// 1/2/3表示谁最小，0表示单个关系中出现了完全相同的元组
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

//打印给定blk的内容addr
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

// getblktem获得某个blk指定第k个的x和y属性(从第0个开始，最多为6)

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
//将blk指定第k个设置为x和y
void SETBLKTEM(unsigned char *blk, int k, int x, int y) {
    char str[5];
    int i;
    itoa(x, str, 10);
    printf("%s\t", str);
    i = k;
    for (int k = 0; k < 4; k++) {
        *(blk + i * 8 + k) = str[k];
    }
    // y=0表示是最后一个后继块
    if (y != 0) {
        itoa(y, str, 10);
        printf("%s\n", str);
        for (int k = 0; k < 4; k++) {
            *(blk + i * 8 + 4 + k) = str[k];
        }
    }
}

void GETBLKTEM_MUL(unsigned char *blk, int k, int *x, int *y, int valid) {
    // valid=1代表无效

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
        "\t1 线性查找ok  \t2 两阶段多路归并排序算法R \t3 "
        "两阶段多路归并排序算法S\t 4建立索引 \t5 索引查R \t 6索引查S "
        "7  关系投影算法\t "
        "8基于排序或散列的两趟扫描算法，实现其中一种集合操作算法");
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
                //仅供测试
                //打印所有元组
                for (int j = 400; j < 432; j++) {
                    if ((blk = readBlockFromDisk(j, &buf)) == NULL) {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                    PRINTBLK(blk);
                    freeBlockInBuffer(blk, &buf);
                }
                // printf("单独测试下215块");
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
            case 1:  //选出R.A=30或S.C=23的元组
            {
                int tempx, tempy;
                // GETBLKTEM(blk, 1, &tempx, &tempy);
                printf("开始选择R.A=30的\n");
                //万一元组超了,这个最大为6
                int tc_size = 0;
                int tc_iosize = 0;
                temp_blk = getNewBlockInBuffer(&buf);

                for (i = 1; i < 17; i++) {
                    blk = readBlockFromDisk(i, &buf);
                    printf("读入数据块%d\n", i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 30) {
                            //我们这里发现了一个元组以后，应该先开辟一个新的缓冲区块，写入这个元组，如果满了的话写入磁盘后重新申请
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
                //如果16个完了以后还有剩下的
                if (tc_size != 0) {
                    writeBlockToDisk(temp_blk, line_count, &buf);
                }
                printf("找到满足选择条件的元组一共%d个\n",
                       tc_iosize * 7 + tc_size);
                printf("总共io%d次\n", buf.numIO);

                line_count += 10;

                printf("\n");

                printf("开始选择S.C=23的\n");
                //万一元组超了,这个最大为6
                tc_size = 0;
                tc_iosize = 0;
                temp_blk = getNewBlockInBuffer(&buf);

                for (i = 17; i < 49; i++) {
                    blk = readBlockFromDisk(i, &buf);
                    printf("读入数据块%d\n", i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 23) {
                            //我们这里发现了一个元组以后，应该先开辟一个新的缓冲区块，写入这个元组，如果满了的话写入磁盘后重新申请
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
                //如果16个完了以后还有剩下的
                if (tc_size != 0) {
                    writeBlockToDisk(temp_blk, line_count, &buf);
                }
                printf("找到满足选择条件的元组一共%d个\n",
                       tc_iosize * 7 + tc_size);
                printf("总共io%d次\n", buf.numIO);

                line_count += 10;
                choice = -1;
                printf("\n");
                break;
            }
            case 2: {
                //实现两阶段多路归并排序算法（TPMMS）：利用内存缓冲区将关系R和S分别排序，并将排序
                //后的结果存放在磁盘上。 对于R,总共是16块，决定6块6块的装
                // 100.blk开始
                int tempx, tempy;
                int tem_array1[42][2] = {0};
                int tc_size = 0;
                int tc_iosize = 0;
                line_count = 100;
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i, &buf);
                    printf("读入数据块%d\n", i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                // for (i = 0; i < 42; i++) {
                //     printf("之前tem_array1 is %d,%d\n", tem_array1[i][0],
                //            tem_array1[i][1]);
                //            }
                SORTTEM(tem_array1, 42);
                //测试一下
                temp_blk = getNewBlockInBuffer(&buf);

                // for (i = 0; i < 42; i++) {
                //     printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                //            tem_array1[i][1]);
                // }
                //新弄块来写入
                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);
                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                // printf("这里我们一共输出了%d块",tc_iosize);
                //最后还有没满的
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // line_count++;

                //然后处理7-13块
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 6, &buf);
                    printf("读入数据块%d\n", i + 6);
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

                    //新弄块来写入

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                //最后还有没满的,哦不对不可能有没满的
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // line_count++;

                // 然后处理13-16块
                for (i = 1; i < 5; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 12, &buf);
                    printf("读入数据块%d\n", i + 12);
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

                    //新弄块来写入

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                //最后还有没满的
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // 解决以后，释放空间
                freeBlockInBuffer(temp_blk, &buf);

                //接下来是多路归并排序
                printf("接下来是多路归并排序\n");
                MULSORT(3, 16, 100, 200, &buf);

                printf("\n");
                choice = -1;
                break;
            }
            case 3: {
                //实现两阶段多路归并排序算法（TPMMS）：利用内存缓冲区将关系R和S分别排序，并将排序
                // 300.blk开始
                printf("\n对s进行排序\n");

                int tempx, tempy;
                int tem_array1[42][2] = {0};
                int tc_size = 0;
                int tc_iosize = 0;
                line_count = 300;
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 16, &buf);
                    printf("读入数据块%d\n", i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        tem_array1[(i - 1) * 7 + j][0] = tempx;
                        tem_array1[(i - 1) * 7 + j][1] = tempy;
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                // for (i = 0; i < 42; i++) {
                //     printf("之前tem_array1 is %d,%d\n", tem_array1[i][0],
                //            tem_array1[i][1]);
                //            }
                SORTTEM(tem_array1, 42);
                //测试一下
                temp_blk = getNewBlockInBuffer(&buf);

                // for (i = 0; i < 42; i++) {
                //     printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                //            tem_array1[i][1]);
                // }
                //新弄块来写入
                printf("\n");

                for (i = 0; i < 42; i++) {
                    printf("tem_array1 is %d,%d\n", tem_array1[i][0],
                           tem_array1[i][1]);
                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        freeBlockInBuffer(temp_blk, &buf);
                        temp_blk = getNewBlockInBuffer(&buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }
                // printf("这里我们一共输出了%d块",tc_iosize);
                //最后还有没满的
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // line_count++;

                //然后处理7-13块
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 6 + 16, &buf);
                    printf("读入数据块%d\n", i + 6);
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

                    //新弄块来写入

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }

                // 然后处理13-18块
                for (i = 1; i < 7; i++) {
                    /* code */
                    blk = readBlockFromDisk(i + 12 + 16, &buf);
                    printf("读入数据块%d\n", i + 12);
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

                    //新弄块来写入

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
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
                    printf("读入数据块%d\n", i + 18);
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

                    //新弄块来写入

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
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
                    printf("读入数据块%d\n", i + 24);
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

                    //新弄块来写入

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
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
                    printf("读入数据块%d\n", i + 30);
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

                    //新弄块来写入

                    SETBLKTEM(temp_blk, tc_size, tem_array1[i][0],
                              tem_array1[i][1]);
                    tc_size++;

                    if (tc_size == 7) {
                        //如果满了的话
                        SETBLKTEM(temp_blk, 7, line_count + 1, 0);

                        writeBlockToDisk(temp_blk, line_count, &buf);
                        line_count++;
                        tc_size = 0;
                        tc_iosize++;
                    }
                }

                //最后还有没满的
                // writeBlockToDisk(temp_blk, line_count, &buf);
                // 解决以后，释放空间
                freeBlockInBuffer(temp_blk, &buf);

                //接下来是多路归并排序
                printf("接下来是多路归并排序\n");
                MULSORT(6, 32, 300, 400, &buf);

                printf("\n");
                choice = -1;
                break;
            }

            case 4: {
                //实现基于索引的关系选择算法：利用（2）中的排序结果为关系R或S分别建立索引文件，
                //利用索引文件选出R.A=30或S.C=23的元组，并将选择结果存放在磁盘上。记录IO读写次数，
                //与（1）中的结果对比。
                // 600开始是r的索引块
                index *indexR = CreateIndex(&buf, 200, 600, 112);
                index *indexS = CreateIndex(&buf, 400, 650, 224);

                choice = -1;
                break;
            }
            case 5: {
                //利用索引文件选出R.A=30或S.C=23的元组，并将选择结果存放在磁盘上。记录IO读写次数
                //首先我要读索引块，找到属性值为30的块号

                int tempx, tempy;
                // GETBLKTEM(blk, 1, &tempx, &tempy);
                printf("开始根据索引选择R.A=30的\n");
                //万一元组超了,这个最大为6
                int tc_size = 0;
                int tc_iosize = 0;
                int tar_addr1, tar_addr2;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag = 0;  // Moren 没找到
                // r的600-605
                i = 0;
                j = 0;
                while (flag != 1 && i < 5) {
                    blk = readBlockFromDisk(600 + i, &buf);
                    printf("读入索引块%d\n", 600 + i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 30) {
                            //我们这里发现了一个元组以后，应该访问其对应的物理块
                            tar_addr1 = tempy;
                            printf("我们找到了一个物理块，块号起始为%d",
                                   tar_addr1);
                            flag = 1;
                            //这里由于我们的文件是顺序排列的，只需要从开始的那个块开始读，直到大于30为止就可以了
                        }
                        if (flag == 1) break;
                    }
                    freeBlockInBuffer(blk, &buf);
                    i++;
                }
                //从tar_addr1开始访问，写入700开始的块里面
                int outputaddr = 700;
                int flag1 = 0;
                i = 0;
                while (flag1 != 1) {
                    blk = readBlockFromDisk(tar_addr1 + i, &buf);
                    printf("读入文件数据块%d\n", tar_addr1 + i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 30) {
                            //发现是30的，写入700
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

                printf("找到满足选择条件的元组一共%d个\n",
                       tc_iosize * 7 + tc_size);
                printf("总共io%d次\n", buf.numIO);

                choice = -1;
                break;
            }

            case 6: {
                int tempx, tempy;
                printf("开始根据索引选择S.C=23的\n");
                //万一元组超了,这个最大为6
                int tc_size = 0;
                int tc_iosize = 0;
                int tar_addr1, tar_addr2;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag = 0;  // Moren 没找到
                // r的600-605
                // s的650-657
                i = 0;
                j = 0;
                while (flag != 1 && i < 7) {
                    blk = readBlockFromDisk(650 + i, &buf);
                    printf("读入索引块%d\n", 650 + i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 23) {
                            //我们这里发现了一个元组以后，应该访问其对应的物理块
                            tar_addr1 = tempy;
                            printf("我们找到了一个物理块，块号起始为%d",
                                   tar_addr1);
                            flag = 1;
                            //这里由于我们的文件是顺序排列的，只需要从开始的那个块开始读，直到大于30为止就可以了
                        }
                        if (flag == 1) break;
                    }
                    freeBlockInBuffer(blk, &buf);
                    i++;
                }
                //从tar_addr1开始访问，写入700开始的块里面
                int outputaddr = 710;
                int flag1 = 0;
                i = 0;
                while (flag1 != 1) {
                    blk = readBlockFromDisk(tar_addr1 + i, &buf);
                    printf("读入文件数据块%d\n", tar_addr1 + i);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        if (tempx == 23) {
                            //发现是23的，写入710
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

                printf("找到满足选择条件的元组一共%d个\n",
                       tc_iosize * 7 + tc_size);
                printf("总共io%d次\n", buf.numIO);

                choice = -1;
                break;
            }
            case 7: {
                //实现关系投影算法：对关系R上的A属性（非主码）进行投影并去重，将结果存放在磁盘上
                //这里直接用排序后的结果200-215
                //存在720上
                int tempx, tempy, oldtempx;
                int outaddr = 720;
                int tc_size = 0;
                int tc_iosize = 0;
                printf("排序完成\n");
                temp_blk = getNewBlockInBuffer(&buf);
                for (i = 1; i < 17; i++) {
                    blk = readBlockFromDisk(i - 1 + 200, &buf);
                    printf("读入数据块%d\n", i - 1 + 200);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);

                        if (i == 1 && j == 0) {
                            oldtempx = tempx;
                            //这里y就随便设成99
                            SETBLKTEM(temp_blk, tc_size, tempx, 99);
                            tc_size++;
                        } else {
                            if (tempx > oldtempx) {
                                oldtempx = tempx;
                                SETBLKTEM(temp_blk, tc_size, tempx, 99);

                                tc_size++;
                            }
                        }
                        //每写7个数据，就应该往磁盘里写了
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
                //如果16个块都处理完了，还有剩的，把下一个的属性值弄成0来区别
                if (tc_size != 0) {
                    if (tc_size < 7) {
                        SETBLKTEM(temp_blk, tc_size, 0, 0);
                    }
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

                    writeBlockToDisk(temp_blk, outaddr, &buf);
                    freeBlockInBuffer(temp_blk, &buf);
                }
                printf("满足去重的属性值共计%d\n", tc_iosize * 7 + tc_size);
                choice = -1;
                break;
            }
            case 8: {
                //实现基于排序的连接操作算法（Sort-Merge-Join）：对关系R和S计算R.A连接S.C，并统计连接次数，将连接结果存放在磁盘上。
                int join_num = 0;
                int tempx, tempy, oldtempx;
                int tempx2, tempy2;
                // R.A连接S.C 730是连接的结果
                // R从200开始 S从400开始
                int is, js;
                int outaddr = 730;
                int tc_size = 0;
                int tc_iosize = 0;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag1 = 0;
                unsigned char *blk_s;
                for (i = 1; i < 17; i++) {
                    blk = readBlockFromDisk(i - 1 + 200, &buf);
                    printf("读入数据块%d\n", i - 1 + 200);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        flag1 = 0;

                        //这里读取了一个blk，接下来遍历S的开始连接
                        is = 1;
                        while ((is < 33) && (flag1 != 1)) {
                            blk_s = readBlockFromDisk(is - 1 + 400, &buf);
                            for (js = 0; js < 7; js++) {
                                GETBLKTEM(blk_s, js, &tempx2, &tempy2);
                                if (tempx == tempx2) {
                                    //连接次数++
                                    join_num++;
                                    SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                                    tc_size++;
                                    //有可能满了
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
                            freeBlockInBuffer(blk_s, &buf);  //还回去
                            is++;
                        }
                        printf("R中的块R.A= %d，总共链接%d次\n", tempx,
                               join_num);
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                //如果16个块都处理完了，还有剩的，把下一个的属性值弄成0来区别
                if (tc_size != 0) {
                    if (tc_size < 7) {
                        SETBLKTEM(temp_blk, tc_size, 0, 0);
                    }
                    SETBLKTEM(temp_blk, 7, outaddr + 1, 0);

                    writeBlockToDisk(temp_blk, outaddr, &buf);
                    freeBlockInBuffer(temp_blk, &buf);
                }

                printf("总共链接%d次\n", join_num);
                choice = -1;
                break;
            }

            case 9: {
                //基于排序或散列的两趟扫描算法，实现其中一种集合操作算法：并、交、差（S-R）中的一种。将结果存放在磁盘上，并统计并、交、差操作后的元组个数。
                printf("我们这里求交集\n");
                //由于前面我们已经有了100-115
                /// 300-331的第一次外排结果，这里直接借用
                //我们直接把R的三个拿进缓冲区，然后分别拿S的进来比较即可
                int tempx, tempy, oldtempx;
                int tempx2, tempy2;
                int join_num = 0;  //相交集合个数
                int is, js;
                int outaddr = 800;
                int tc_size = 0;
                int tc_iosize = 0;
                temp_blk = getNewBlockInBuffer(&buf);
                int flag1 = 0;
                unsigned char *blk_s;
                for (i = 1; i < 17; i++) {
                    blk = readBlockFromDisk(i - 1 + 200, &buf);
                    printf("读入数据块%d\n", i - 1 + 200);
                    for (j = 0; j < 7; j++) {
                        GETBLKTEM(blk, j, &tempx, &tempy);
                        flag1 = 0;

                        //这里读取了一个blk，接下来遍历S的开始连接
                        is = 1;
                        while ((is < 33) && (flag1 != 1)) {
                            blk_s = readBlockFromDisk(is - 1 + 400, &buf);
                            for (js = 0; js < 7; js++) {
                                GETBLKTEM(blk_s, js, &tempx2, &tempy2);
                                if ((tempx == tempx2) && (tempy == tempy2)) {
                                    //相交次数++
                                    join_num++;
                                    SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                                    tc_size++;
                                    //有可能满了
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
                            freeBlockInBuffer(blk_s, &buf);  //还回去
                            is++;
                        }
                        printf("R中的块R.A= %d，总共相交%d次\n", tempx,
                               join_num);
                    }
                    freeBlockInBuffer(blk, &buf);
                }
                //如果16个块都处理完了，还有剩的，把下一个的属性值弄成0来区别
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
                //基于排序或散列的两趟扫描算法，实现其中一种集合操作算法：并、交、差（S-R）中的一种。将结果存放在磁盘上，并统计并、交、差操作后的元组个数。
                printf("我们这里求并集\n");
                //由于前面我们已经有了100-115
                /// 300-331的第一次外排结果，这里直接借用
                //我们直接把R的三个拿进缓冲区，然后分别拿S的进来比较即可
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

                //先读S的，用去1个块，这里直接用读入这1个块的结果
                //由于此时缓冲区只剩下7块，所以R要6块的读
                printf("开始写入块%d\n", outaddr);
                for (i = 1; i < 33; i++) {
                    blk = readBlockFromDisk(i - 1 + 400, &buf);
                    printf("读入S的数据块%d\n", i - 1 + 400);
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
                            //证明SbingR里面这个不用写两次
                            SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                            tc_size++;
                            //有可能满了
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
                //然后把R也写进去就好了
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

                printf("结果最终写入了%d - %d,s并r总共%d个元组", 810, outaddr,
                       length2 + length1 - effect_num);

                choice = -1;
                break;
            }
            case 11: {
                //基于排序或散列的两趟扫描算法，实现其中一种集合操作算法：并、交、差（S-R）中的一种。将结果存放在磁盘上，并统计并、交、差操作后的元组个数。
                printf("我们这里求差（S-R）集\n");
                //由于前面我们已经有了100-115
                /// 300-331的第一次外排结果，这里直接借用
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

                //先读S的，用去1个块，这里直接用读入这1个块的结果
                //由于此时缓冲区只剩下7块，所以R要6块的读
                printf("开始写入块%d\n", outaddr);
                for (i = 1; i < 33; i++) {
                    blk = readBlockFromDisk(i - 1 + 400, &buf);
                    printf("读入S的数据块%d\n", i - 1 + 200);
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
                            //证明S-R里面没有这个
                            SETBLKTEM(temp_blk, tc_size, tempx, tempy);
                            tc_size++;
                            //有可能满了
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

                printf("结果最终写入了%d - %d,s-r总共%d个元组", 900, outaddr,
                       length2 - effect_num);

                choice = -1;
                break;
            }
        }
    }
}

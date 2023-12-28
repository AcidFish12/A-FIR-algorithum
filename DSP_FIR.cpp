// DSP_FIR.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <fstream>

#define BUFFER_SIZE 1024
#define NUM_THREADS 4
#define FILTER_LENGTH 32
#define PI 3.1415926535

void file_write(double* filedata, const char* str);
double input_buffer[BUFFER_SIZE];
double output_buffer[BUFFER_SIZE];
double filter_coefficients[FILTER_LENGTH] = {
    -5.061877164e-05, -9.336374205e-05,  0.0003678254143,   0.0005914540961, -0.001496405224,
          -0.002226336394,   0.004491201602,   0.006349918433,   -0.01111812051,   -0.01528073754,
           0.02449801192,    0.03380065784,   -0.0527959317,     -0.0793820098,     0.1454829872,
           0.4468590915,     0.4468590915,     0.1454829872,     -0.0793820098,    -0.0527959317,
           0.03380065784,    0.02449801192,   -0.01528073754,    -0.01111812051,    0.006349918433,
           0.004491201602,  -0.002226336394,  -0.001496405224,    0.0005914540961,  0.0003678254143,
          -9.336374205e-05, -5.061877164e-05 }; // 滤波器系数

DWORD WINAPI FilterThread(LPVOID lpParam)
{
    int thread_index = (int)lpParam;
    int start_index = thread_index * (BUFFER_SIZE / NUM_THREADS);
    int end_index = (thread_index + 1) * (BUFFER_SIZE / NUM_THREADS) - 1;

    for (int i = start_index; i <= end_index; i++)
    {
        output_buffer[i] = 0.0;
        for (int j = 0; j < FILTER_LENGTH; j++)
        {
            int input_index = i - j + (FILTER_LENGTH / 2);
            if (input_index < 0 || input_index >= BUFFER_SIZE)
                continue;
            output_buffer[i] += input_buffer[input_index] * filter_coefficients[j];
        }
    }

    return 0;
}

int main()
{
    HANDLE hThreads[NUM_THREADS];
    DWORD dwThreadIds[NUM_THREADS];
    int Fs = 1000;
    // 生成随机信号
    unsigned int i;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        input_buffer[i] = 5 * sin(2 * PI * 100 * ((double)i / Fs)) + 15 * sin(2 * PI * 450 * ((double)i / Fs));
    }


    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++)
    {
        hThreads[i] = CreateThread(NULL, 0, FilterThread, (LPVOID)i, 0, &dwThreadIds[i]);
    }

    // 等待线程结束
    WaitForMultipleObjects(NUM_THREADS, hThreads, TRUE, INFINITE);

    // 输出结果
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%lf\n", output_buffer[i]);
    }

    // 关闭线程句柄
    for (int i = 0; i < NUM_THREADS; i++)
    {
        CloseHandle(hThreads[i]);
    }
    //滤波前数据存入txt
    const char* input_path;
    input_path = "inputData.txt";
    file_write(input_buffer, input_path);
    //滤波后数据存入txt
    const char* output_path;
    output_path = "outputData.txt";
    file_write(output_buffer, output_path);

    return 0;
}
void file_write(double* filedata, const char* str)
{
    double* ini = (double*)calloc(BUFFER_SIZE, sizeof(double));
    double* ini2 = ini;
    FILE* FilePosition;
    fopen_s(&FilePosition, str, "w");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        *(ini2) = filedata[i];
        ini2++;
    }
    //Write File Operation
    for (int j = 0; j < BUFFER_SIZE; j++) {
        fprintf(FilePosition, "%lf ", *(ini + j));
    }
    fclose(FilePosition);
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

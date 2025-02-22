#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_COMMAND_LENGTH 256

// 检查命令是否已经存在于文件中
int command_already_successful(const char *command, const char *success_file) {
    FILE *file = fopen(success_file, "r");
    if (!file) return 0;

    char line[MAX_COMMAND_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, command)) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// 执行单个命令并记录结果
void execute_command(const char *command, const char *success_file, const char *failure_file) {
    int status = system(command);
    FILE *file;

    if (status == 0) {
        file = fopen(success_file, "a");
    } else {
        file = fopen(failure_file, "a");
    }

    fprintf(file, "%s", command);
    fclose(file);
}

// 线程任务结构体
typedef struct {
    char command[MAX_COMMAND_LENGTH];
    const char *success_file;
    const char *failure_file;
} Task;

// 线程执行函数
void *worker_thread(void *arg) {
    Task *task = (Task *)arg;
    execute_command(task->command, task->success_file, task->failure_file);
    free(task);
    return NULL;
}

// 读取命令文件并启动线程
void read_commands(const char *command_file, const char *success_file, const char *failure_file, int cpu) {
    FILE *file = fopen(command_file, "r");
    if (!file) {
        printf("Error: Could not open command file\n");
        exit(1);
    }

    char line[MAX_COMMAND_LENGTH];
    pthread_t threads[cpu];
    int thread_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // 去除换行符

        if (command_already_successful(line, success_file)) {
            printf("Skipping: %s\n", line);
        } else {
            printf("Executing: %s\n", line);

            // 创建任务
            Task *task = (Task *)malloc(sizeof(Task));
            strcpy(task->command, line);
            task->success_file = success_file;
            task->failure_file = failure_file;

            // 创建线程
            pthread_create(&threads[thread_count % cpu], NULL, worker_thread, task);
            thread_count++;

            // 如果达到 CPU 数量，等待线程完成
            if (thread_count % cpu == 0) {
                for (int i = 0; i < cpu; i++) {
                    pthread_join(threads[i], NULL);
                }
            }
        }
    }

    // 等待剩余的线程完成
    for (int i = 0; i < thread_count % cpu; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(file);
}

// 主函数
int main(int argc, char *argv[]) {
    // 解析命令行参数
    char *command_file = NULL;  // -c 指定的命令文件
    int cpu = 1;                // -CPU 指定的并行数
    char *success_file = NULL;  // -s 指定的成功文件
    char *failure_file = NULL;  // -f 指定的失败文件

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            command_file = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-CPU") == 0 && i + 1 < argc) {
            cpu = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            success_file = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            failure_file = argv[i + 1];
            i++;
        }
    }

    // 检查参数是否有效
    if (!command_file || !success_file || !failure_file || cpu <= 0) {
        printf("Usage: %s -c <command_file> -CPU <num> -s <success_file> -f <failure_file>\n", argv[0]);
        return 1;
    }

    // 读取命令并执行
    read_commands(command_file, success_file, failure_file, cpu);

    printf("All commands processed.\n");
    return 0;
}
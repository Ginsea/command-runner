# **Command Runner**

**Command Runner** 是一个用 C 语言编写的命令行工具，用于批量执行 Linux 命令，并根据执行结果将命令记录到不同的文件中（成功或失败）。支持多线程并行执行，适用于自动化任务处理。

---

## **功能**
1. **批量执行命令**：从文件中逐行读取命令并执行。
2. **多线程支持**：通过指定线程数并行执行命令。
3. **记录执行结果**：将成功和失败的命令分别记录到不同的文件中。
4. **命令过滤**：如果命令已经存在于成功文件中，则跳过执行。

---

## **安装**

1. **克隆仓库**：
   ```bash
   git clone https://github.com/yourusername/command-runner.git
   cd command-runner
   ```

2. **编译程序**：
   ```bash
   gcc -o CommandRunner command_runner.c -lpthread
   ```

3. **确保程序可执行**：
   ```bash
   chmod +x CommandRunner
   ```

---

## **使用方法**

### **命令行参数**
运行程序时需要以下参数：
```bash
./CommandRunner -c <command_file> -CPU <num> -s <success_file> -f <failure_file>
```

- **`-c <command_file>`**：包含命令的文件路径，每行一个命令。
- **`-CPU <num>`**：并行执行的线程数。
- **`-s <success_file>`**：成功记录文件路径。
- **`-f <failure_file>`**：失败记录文件路径。

### **示例**
假设：
- 命令文件：`commands.txt`
- 并行线程数：`4`
- 成功记录文件：`success.txt`
- 失败记录文件：`failure.txt`

运行命令：
```bash
./CommandRunner -c commands.txt -CPU 4 -s success.txt -f failure.txt
```

---

## **文件格式**

### **命令文件 (`commands.txt`)**
每行一个命令。例如：
```bash
echo "Command 1"
sleep 2
echo "Command 2"
ls -l
```

### **成功记录文件 (`success.txt`)**
成功执行的命令会被追加到该文件。

### **失败记录文件 (`failure.txt`)**
执行失败的命令会被追加到该文件。

---

## **示例**

1. **创建工作目录**：
   ```bash
   mkdir -p ./command_runner
   cd ./command_runner
   ```

2. **创建命令文件**：
   ```bash
   echo 'echo "Hello, World!"
   sleep 1
   ls -l
   invalid_command' > commands.txt
   ```

3. **运行程序**：
   ```bash
   ./CommandRunner -c commands.txt -CPU 2 -s success.txt -f failure.txt
   ```

4. **检查结果**：
   - **`success.txt`**：
     ```bash
     echo "Hello, World!"
     ls -l
     ```
   - **`failure.txt`**：
     ```bash
     invalid_command
     ```

---

## **依赖**
- **GCC 编译器**：用于编译 C 代码。
- **pthread 库**：用于支持多线程。

---

## **许可证**
本项目采用 [MIT License](LICENSE)。

---

## **贡献**
欢迎提交 Issue 或 Pull Request 改进本项目！

---

## **联系方式**
如有问题，请联系：
- 邮箱：cginsea@gmail.com
- GitHub: [yourusername](https://github.com/yourusername)
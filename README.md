<!--
 * @Author: Tian Wei
 * @Date: 2021-09-10 19:37:27
 * @Description: 
 * @Version: V1.0
-->

# FreeRTOS_HAL_ARMFLY_V5

 使用安富莱STM32_V5开发板进行HAL库的FreeRTOS学习

# FreeRTOS命名规则

## 变量

1. uint32_t定义的变量都加上前缀ul。u代表unsigned 无符号，l代表long长整型
2. uint16_t定义的变量都加上前缀us。u代表unsigned无符号，s代表short短整型。
3. uint8_t定义的变量都加上前缀uc。u代表unsigned无符号，c代表char字符型。
4. stdint.h文件中未定义的变量类型，在定义变量时需要加上前缀x，比如BaseType_t和TickType_t定义的变量。
5. stdint.h文件中未定义的无符号变量类型，在定义变量时要加上前缀u，比如UBaseType_t定义的变量要加上前缀ux。
6. size_t 定义的变量也要加上前缀ux。
7. 枚举变量会加上前缀e。
8. 指针变量会加上前缀p，比如uint16_t定义的指针变量会加上前缀pus。
9. 根据MISRA代码规则，char定义的变量只能用于ASCII字符，前缀使用c。
10. 根据MISRA代码规则，char *定义的指针变量只能用于ASCII字符串，前缀使用pc。

# 函数

1. 加上了static声明的函数，定义时要加上前缀prv，这个是单词private的缩写。
2. 带有返回值的函数，根据返回值的数据类型，加上相应的前缀，如果没有返回值，即void类型 ，函数的前缀加上字母v。
3. 根据文件名,文件中相应的函数定义时也将文件名加到函数命名中,比如tasks.c文件中函数vTaskDelete,函数中的task就是文件名的task。

# 宏定义

1. 根据宏定义所在的文件，文件中的宏定义声明时也将文件名加到宏定义中，比如宏定义configUSE_PREEMPTION 是定义在文件
2. FreeRTOSConfig.h里面。宏定义中的config就是文件名中的config。另外注意，前缀要小写。  除了前缀，其余部分全部大写，同时用下划线分开

# FreeRTOS中数据类型

## TickType_t

如果用户使能了宏定义 configUSE_16_BIT_TICKS，那么TickType_t定义的就是16位无符号数，如果没有使能，那么TickType_t定义的就是32位无符号数。对于32位架构的处理器，一定要禁止此宏定义，即设置此宏定义数值为0即可。

## BaseType_t

这个数据类型根据系统架构的位数而定，对于32位架构，BaseType_t定义的是32位有符号数，对于16位架构，BaseType_t定义的是16位有符号数。如果BaseType_t被定义成了char型，要特别注意将其设置为有符号数，因为部分函数的返回值是用负数来表示错误类型。

## UBaseType_t

这个数据类型是BaseType_t类型的有符号版本。

## StackType_t

栈变量数据类型定义，这个数量类型由系统架构决定，对于16位系统架构，StackType_t定义的是 16位变量，对于32位系统架构，StackType_t定义的是32位变量。

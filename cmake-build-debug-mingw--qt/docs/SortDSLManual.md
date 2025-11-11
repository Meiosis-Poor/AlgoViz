#  Sort DSL 使用手册

## 1. 概述
**Sort DSL** 是一门用于控制排序可视化组件（SortWidget）的简易脚本语言。  
通过 Sort DSL，你可以方便地：
- 定义与操作排序数据序列
- 插入、删除、清空元素
- 设置排序方式与顺序（升序/降序）
- 自动运行或暂停排序动画
- 使用变量与循环批量构建测试数据

支持功能包括：
- 基本数据操作（插入、删除、重置、清空）
- 排序算法选择与执行
- 升降序控制
- 自动运行控制
- 变量与循环语法

---

## 2. 基本语法

### 2.1 变量定义
使用 `let` 定义整数变量：
```dsl
let n = 5
let interval = 1000
```
说明：
- 变量名只能包含字母、数字或下划线
- 赋值必须为整数
- 所有变量为全局作用域，可在任意行引用

### 2.2 循环
使用 `for` 执行循环块：
```dsl
for i in range(0, n)
insert(i, i * 2)
end
```
语法说明：
- `range(start, end)` 表示从 `start` 到 `end - 1`
- 循环块以 `end` 结尾
- 循环变量仅在循环体内定义，但值会覆盖同名全局变量

示例：
```dsl
let n = 5
for i in range(0, n)
insert(i, i)
end
```

## 3. 命令说明

### 3.1 数据操作命令

| 命令 | 说明 | 示例 |
|------|------|------|
| `insert(value)` | 在数组末尾插入值 | `insert(5)` |
| `insert(pos, value)` | 在指定位置插入值 | `insert(2, 10)` |
| `remove(index)` | 删除指定下标的元素 | `remove(3)` |
| `clear()` | 清空全部数据 | `clear()` |
| `reset()` | 重置为初始状态 | `reset()` |

### 3.2 排序算法命令

| 命令 | 说明 |
|------|------|
| `sort()` | 执行当前选择的排序算法 |
| `sortBubble()` | 冒泡排序 |
| `sortInsertion()` | 插入排序 |
| `sortSelection()` | 选择排序 |
| `sortQuick()` | 快速排序 |
| `sortMerge()` | 归并排序 |
| `sortHeap()` | 堆排序 |

示例：
```dsl
insert(3)
insert(1)
insert(4)
insert(2)
sortQuick()
```

### 3.3 排序方向控制

| 命令 | 参数 | 说明 | 示例 |
|------|------|------|------|
| `setAscending(flag)` | 1/true 为升序，0/false 为降序 | 设置排序方向 | `setAscending(true)` |

### 3.4 自动运行与控制

| 命令 | 参数 | 说明 | 示例 |
|------|------|------|------|
| `autoRun(interval)` | 毫秒为单位的自动播放间隔 | 启动自动排序播放 | `autoRun(500)` |
| `stop()` | 无参数 | 停止自动播放 | `stop()` |

## 4. 示例

### 4.1 创建序列并执行冒泡排序
```dsl
clear()
insert(3)
insert(1)
insert(4)
insert(2)
sortBubble()
```

### 4.2 使用变量与循环批量生成数据
```dsl
let n = 5
clear()
for i in range(0, n)
insert(i, n - i)
end
setAscending(true)
sortSelection()
```

### 4.3 自动播放动画
```dsl
clear()
for i in range(0, 10)
insert(i, 10 - i)
end
setAscending(true)
autoRun(800)
```

## 5. 注意事项
- `insert` 的位置索引从 0 开始计数
- 索引超出范围或数据为空会被忽略或报错提示
- 所有命令参数必须为整数或已定义变量
- DSL 不支持浮点数或字符串
- `autoRun` 与 `stop` 仅影响排序动画，不改变排序结果

## 6. 高级功能

### 6.1 嵌套循环
可用嵌套循环快速生成复杂序列：
```dsl
for i in range(0, 3)
for j in range(0, 2)
insert(i * 2 + j)
end
end
sortMerge()
```

### 6.2 灵活组合命令
DSL 可灵活组合数据生成与排序控制：
```dsl
let n = 10
clear()
for i in range(0, n)
insert(i, n - i)
end
setAscending(false)
sortHeap()
```
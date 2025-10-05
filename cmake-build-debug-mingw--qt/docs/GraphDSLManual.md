# Graph DSL 使用手册

## 1. 概述
Graph DSL 是用于操作图结构的简易脚本语言。你可以通过它快速执行图的节点和边操作、搜索算法和最短路径算法。

**支持功能：**
- 图节点的增删
- 边的增删
- BFS / DFS / Dijkstra / Prim / Kruskal
- 变量定义
- 循环

## 2. 基本语法

### 2.1 变量定义
使用 `let` 定义变量：
```dsl
let a = 5
let startNode = 0
```
**注意：**
- 变量名只能包含字母、数字或下划线
- 赋值只能为整数

### 2.2 循环
使用 for 循环：
```dsl
for i in range(0, 3)
bfs(i)
end
```

**语法说明：**
- `range(start, end)` 表示从 start 到 end - 1
- 循环块使用 `end` 结尾
- 循环块内部可以使用循环变量

## 3. 命令说明

### 3.1 节点操作
- `insertNode()` : 插入一个新节点
- `deleteNode(id)` : 删除节点 id

### 3.2 边操作
- `addEdge(u, v, weight)` : 在节点 u 和 v 之间添加边，权值为 weight
- `deleteEdge(u, v)` : 删除节点 u 到 v 的边

### 3.3 图算法
- `bfs(start)` : 从 start 节点执行广度优先搜索
- `dfs(start)` : 从 start 节点执行深度优先搜索
- `dijkstra(start)` : 从 start 节点计算最短路径
- `prim()` : 执行 Prim 最小生成树算法
- `kruskal()` : 执行 Kruskal 最小生成树算法
- `random(nodes, edges, density)` : 随机生成图，节点数 nodes，边数 edges，密度 density

## 4. 示例

### 4.1 创建图并添加边
```dsl
insertNode()
insertNode()
insertNode()
addEdge(0, 1, 5)
addEdge(1, 2, 3)
```

### 4.2 使用变量和循环
```dsl
let startNode = 0
for i in range(0, 3)
bfs(i)
end
```

## 5. 注意事项
- 节点编号必须在当前图范围内，否则会报错
- 循环内部可以使用变量，但变量作用域为整个脚本
- 命令参数必须为整数或已定义变量
- DSL 不支持浮点数（除 random 的密度参数外）

## 6. 高级功能

### 6.1 嵌套循环
```dsl
for i in range(0, 2)
for j in range(0, 2)
addEdge(i, j, 1)
end
end
```

### 6.2 其他特性
- 支持多次调用算法函数
- 可以组合变量和命令，灵活操作图结构
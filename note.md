# scanner
    - 扫描文件
    - 划分token

# parser
    - 解析token
    - 生成抽象语法树

# 语法树优先级问题
    1. 显式
    2. 运算符优先级表

# 生成汇编代码

# 语句

# 变量
    定义变量        cgglobsym()
    变量 >> 寄存器  cgloadglob()
    寄存器 >> 变量  cgstorglob()

# 比较运算符

# if-else语句
    1. 没有else语句，Lfalse表示if结束后的语句开始标志
        生成if判断语句，并判断
        如果if语句判断为false，则跳转到Lfalse
        生成true处理语句段段
        生成Lfalse语句段
    2. 如果有else语句，Lfalse表示else语句段
        生成if判断语句，并判断
        如果if语句判断为false，则跳转到Lfalse
        生成true处理语句段段
        生成跳转到Lend语句
        生成Lfalse标签
        生成Lfalse语句段
        生成Lend标签


# 其他
    * expr.c
        - 生成表达式AST节点
            - primary函数：生成INTLIT和IDENT节点
    * scan.c
        - 扫描token
    * stmt.c
        - 按语句生成AST节树

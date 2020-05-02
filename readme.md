# Readme

[TOC]

## 编译方法

* For Linux:
  * 将lib中的.so文件移动到/usr/lib目录，再把main中的Makefile_b去掉后缀，make编译运行

* For Windows:
  * 将lib中的.dll和.lib文件移动到main中，再把main中的Makefile_a去掉后缀，make编译运行


## 开发日志

* 2020.04.13 - 更早

  更新readme，实现大数，加密，哈希

## 项目简介

**项目名：** Prometheus
**项目内容：** 模拟区块链
为什么要重复造轮子呢？我也不知道。。。

## 功能设计

### 加密算法

#### RSA

##### 大数类

* （参考项目https://github.com/thuliangjz/rsa-DIY）

1. 2^64 进制存储数据，减少位数

2. 加减乘法汇编加速，一次可以处理多位数字与进位溢出问题

   - 理论上处理器支持 128 位寄存器，可以进一步扩大进制

   - 加法每次即每次从寄存器 `EFLAGS` 中读取上一次相加所得进位，用 `adc` 指令将数和上一次进位 `CF` 相加，保存进位增加索引进入下一轮相加，核心代码如下：

   

``` asm
    sahf;
    movq (%%r8, %%rcx, 8), %%rax;
    movq (%%r9, %%rcx, 8), %%rbx;
    adc %%rax, %%rbx;
    lahf;
    movq %%rbx, (%%r10, %%rcx, 8);
```

   - 减法即将 `adc` 改为 `sub` 即可

   - 乘法模拟竖式，先将一个乘数中的某一个整型乘以另一个大整数，移位后加到最终结果上。第一步代码如下：

   

``` asm
    mov %%r8, %%rax;	//小乘数保存在r8中
    mov (%%rsi, %%rcx, 8), %%rbx;	//取出大乘数
    mul %%rbx;
    mov %%rax, %%rbx;
    ASM_RESTORE
    adc %%r14, %%rbx;	//本轮结果即本次乘法+上次乘法溢出+上次加法溢出
    ASM_LOAD
    mov %%rdx, %%r14;	//mul指令溢出保存在r14中
    mov %%rbx, (%%rdi, %%rcx, 8);
```

   - 除法为长除法。这里实现的实际上是基于二进制的整形除法，每次确定商的一个比特位。长除法中的乘法全部被移位所替代，因为每次除数需要乘的都是 2 的幂次，这在一定程度上加快了除法的速度

3. 快速幂借助 Newton-Raphson 算法，用乘法和加减法代替除法。用定点数存储，以调用之前的整数运算实现小数运算

   - 精度问题在原项目中有详细分析，摘抄如下

     - 关于定点数的精度，对于计算 $a^b \pmod{n}$，在使用快速幂算法进行迭代计算的过程中，每一步实际上都是对于某个$x \le n$计算$x^2 \pmod n$，所以实际上被除数不大于$n^2$。如果$n$有$p$位，则$n^2$不超过$2p$位，从而对于这样的$x^2$，逆的精度只要精确到小数点后$2p$位即误差不超过$2^{-2p}$即可保证与被除数相乘与商的误差不超过 1。
     - 注意到如果将$\lfloor \frac 1n \times x \rfloor $作为商，即使误差本身不超过 1，向下取整之后可能会达到 1（向下取整在这里实现为移位运算），从而按上述方法计算的结果可能需要减去 n 才能得到真正比 n 小的余数。

##### 验证质数

1. 采用方案：Miller-Rabin 检测（伪代码）

   Miller-Rabin 素性检验是一个基于概率的检验方案，与费马素性检验相比条件更强，更不易出现类似 Carmichael 数一类的漏网之鱼；而 AKS 素性检验可以确定地判断给定数字是否为素数，但是速度相较 Miller-Rabin 更慢。

   该素性检验的主要思想为：

   待检验的数为 $n$ （$n$为奇数）， 将 $n-1$ 分解为 $2^r \cdot d$, 其中 $d$ 为奇数。若$\exists \; a \in [2, n-2], \; a\in \mathrm{N}\; s.t.\forall 0 \leq s \leq r-1, \; a^d \not\equiv 1(mod\; n)\; \text{且}\; a^{2^sd} \not\equiv n-1(mod \; n), $则$n$为合数。通过$k$轮检验($i.e.\; k$ 次随机选取$a$)。以下为该检验方法的伪代码：

   write $n-1$ as $2^r \cdot d$ by factoring powers of $2$ from $n-1$

   Loop: repeat $k$ times:

    	pick a random integer $a\in [2, n-2]$

    	$x \leftarrow\;a^d \; mod\; n$

    	if $x = 1$ then

    		continue Loop

    	repeat $r$ times:

    		if $x = n-1$ then

    			continue Loop

    		$x \leftarrow\; x^2\;mod\;n$

   ​	 return composite
   
   return probably prime

参考密码学资料以及 Java 的 RSA 库，为了平衡效率和正确率，1024 位 RSA 将选取$k=4$.

##### RSA 实现

* RSA 生成公私钥的步骤如下

  1. 任选两个 512 位素数$p, q$, 计算其积为$N$
  2. 计算欧拉数$\varphi = (p-1)\cdot (q-1)$
  3. 任选 $e$ 满足 $1<e<\varphi$ 且 $gcd(e, \varphi) = 1$, $(e, N)$即为公钥。实际应用中 $e$ 大多选择 $3$ 或 65537
  4. 任选 $d$ 满足 $1<d<\varphi$ 且 $e\cdot d \equiv 1(mod\; \varphi)$, $( d, N)$即为私钥。

  在步骤 a. 中，为了加快素数生成速度，先随机生成一个奇数 $n$, 取以 $n$ 为首项的 1000 个连续奇数，用小于 10000 的素数筛一遍，剩下的数用 Miller-Rabin 素性检验进行检测，如果不存在素数，则重复上述过程。

  在步骤 d. 中，采用扩展欧几里得算法计算 $d\; mod \; N$ 的逆元。

* RSA 加解密

  + 公钥加密，私钥解密

    $\text{密文} = \text{明文}^e \; mod \; N$

    $\text{明文} = \text{密文}^d\;mod\;N$

  + 私钥加密，公钥解密（用于交易中的数字签名及验证，详细介绍见后文）

    $\text{密文} = \text{明文}^d \; mod \; N$

    $\text{明文} = \text{密文}^e\;mod\;N$

### 哈希算法

#### SHA256

* 摘抄自[sha256](http://www.zedwood.com/article/cpp-sha256-function)，这就不重复造轮子了

#### ripemd160

* 摘抄自[ripemd160](https://homes.esat.kuleuven.be/~bosselae/ripemd160/)，这就不重复造轮子了（哈希算法似乎没有什么自己写的意义）

### p2p 网络

*方便起见下文中交易发起方称为Bob，接收方称为Alice*

#### 共享内容

##### 交易池

1. 交易池内存储所有交易记录（打包进区块链的和等待矿工打包的）

2. 交易记录：

   - 交易信息结构

     1. 交易 ID：本次交易的编号
     2. Bob地址
     3. Alice地址
     4. 交易输入：Bob信息

        1. 交易 ID：依赖交易

        2. Bob的公钥

        3. 数字签名

        4. 金额

     5. 交易输出：Alice信息

        1. Alice公钥哈希（见下文 `publickeyHash` ）
        2. 金额
* 特别地，在 Coinbase 交易（i.e. 挖矿获得奖励）中，不存在交易输入。

   - 数字签名
   

  + 签名方式：Bob私钥加密公钥解密

     - 签名内容
       - Bob的公钥哈希
       - Alice的公钥哈希
       - 交易金额

   - UTXO 模型

* 交易2中，B使用了交易1中输出0指向的代币，即输入1中交易2的前向交易为交易1
      - 交易2中，C还没有产生新的交易，即输出1还没有被花费，即未花费的交易输出(UTXO)

![tx.png](https://github.com/TeamFromNFLS/BlockChain/blob/master/Doc/reports/tx.png?raw=true)

##### 区块链

##### 添加新节点

1. 生成地址

2. 同步区块链

3. 验证区块链完整性（验证梅克尔树树根与交易记录是否一致，如不一致遍历梅克尔树找到错误交易重新同步）

   - 梅克尔树

     

![img](https://upload-images.jianshu.io/upload_images/4272913-a36435a35415fe9d.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

 - 存储打包的每条交易记录
 - 若重构梅克尔树后树根哈希值不同，则广度优先遍历每个节点，最终可以在$O(\log n)$复杂度内找到出错的交易记录

​     

#### 节点

##### 普通节点（钱包）

1. 钱包地址生成

   1. 随机生成两个512位的质数$p, q$，计算乘积$N$，将数对$<e = 65537, n>$设为公钥 `publickey` 

   2. 计算模数为$\varphi$下$e$的逆元$d$, 将数对$<d, n>$设为私钥 `privatekey` 

   3. 对 `publickey` 进行 sha256 散列和 ripemd160 散列得到 `publickeyHash` 

   4. 在 `publickeyHash` 头添加 version 字节得到 `versionPublickeyHash` 

   5. 对 `versionPublickeyHash` 进行两次 sha256 散列取前四位字节得到 `tailHash` 

   6. 在 `versionPublickeyHash` 尾添加 `tailHash` 得到 `finalHash` 

   7. 对 `finalHash` 进行 Base58 编码得到地址

2. 交易生成广播

   1. 查询足够本次交易的 UTXO

      1. 遍历交易池得到 sender 为当前发起者地址的交易（i.e. 这些交易花费了之前生成的 UTXO），将这些交易所依赖的交易（i,e. 交易输入中的 ID）记录到 setA 中
      2. 遍历区块链，排除 setA 中所有已经打包的交易，剩下即为可以依赖的交易

   2. 生成交易记录发送进交易池，等待矿工检验并打包完成交易（此时默认 UTXO 已经花费，避免二次支付）

3. 查询本钱包地址余额（回溯区块链中交易）

##### 矿工节点（继承普通节点）

1. 打包交易池中交易进区块链（工作量证明）

   - 目标值：最大目标值 / 难度值

* 打包验证通过后更新 `prevBlockHash` （声明见下）

   
  

![mine.png](https://github.com/TeamFromNFLS/BlockChain/blob/master/Doc/reports/mine.png?raw=true)

2. 验证交易信息

   1. 地址和公钥是否相符
   2. 交易信息和数字签名是否相符

3. 验证区块

   1. 验证工作量证明：是否小于目标值
   2. 验证梅克尔树树根与交易记录是否一致
   3. 验证是否只有一个挖矿奖励交易
   4. 验证所有交易是否合法

### 区块链

#### 区块头

| **字段**         | **大小** | **描述**                           |
| ---------------- | -------- | ---------------------------------- |
| version          | 4 字节   | 版本号，⽤于跟踪软件/协议的更新     |
| prevBlockHash    | 32 字节  | 上一个区块的哈希地址             |
| merkleRoot       | 32 字节  | 该区块中交易的梅克尔树根的哈希值 |
| time             | 4 字节   | 该区块的创建时间戳                 |
| difficultyTarget | 4 字节   | 该区块链工作量证明难度目标         |
| nonce            | 4 字节   | 用于证明工作量的计算参数           |

#### 区块体

| **字段**             | **大小**   | **描述**               |
| -------------------- | ---------- | ---------------------- |
| numTransactionsBytes | 1 字节     | 交易数量占用的字节数   |
| numTransactions      | 0-8 个字节 | 区块内存储的交易数量   |
| transactions         | 不确定     | 区块内存的多个交易数据 |

## 组员分工

* 葛依然
  + 实现RSA算法
  + 实现交易
* 李家祺
  + 实现p2p网络模拟
* 潘浩
  + 封装大数库
  + 实现区块链和工作量证明
  + 催促其他组员完成工作（不是）


















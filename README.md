# wrapper-esp32

自用esp-idf框架下的C++风格组件封装

主要对常用esp-idf本体组件和在线注册组件进行封装


# 为什么封装?

主要简化以下步骤的操作

- esp-idf 初始化结构体 xxx_config_t 很多都结构复杂, 封装后在新的 XxxConfig 构造函数处一次性填完, 减少结构体嵌套填参的麻烦
- 部分组件初始化步骤较多, 适当封装偷懒

# 主要模块

- wrapper: esp-idf 组件封装, esp32本身功能, 中间件接口, 以及一些纯软组件
- device: 建立在wrapper之上, 对各中外设进行封装
- board:  集合核心总线和IO初始化, 外设初始化的板级代码, 在开发app时, hal层直接找board单例对硬件进行调用

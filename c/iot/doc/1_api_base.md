[TOC]


# 账号服务API

## 错误码

| 错误号      | 说明               |
| -------- | ---------------- |
| 20000001 | 用户未登录            |
| 20000002 | 用户不在此群组，也不在上级群组  |
| 20000003 | 无此资源权限记录         |
| 20000004 | 权限不足             |
| 20000005 | secret不合法        |
| 20000006 | 未找到nounce        |
| 20000007 | 未找到设备类型对应的secret |
| 20000008 | 用户名或密码错误         |
| 20000009 | 被删用户必须在指定群组中     |
| 20000010 | 不能操作高等级角色        |
| 20000011 | 角色不存在            |
| 20000012 | ID不合法            |
| 20000013 | 用户已存在            |

## 权限认证

### /wapi/auth/device/*

#### 获取鉴权随机数

- URI: `/wapi/auth/device/nounce`

- Method: `GET`

- Parameter:

  ```json
  {
    "Device": "W001_P13866886688",	// 设备ID, 包含密钥类型
    "Secret": "a83b83ca9259c1sf8"		// 根据密钥类型对应的密钥与设备ID hash 生成
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": {
      "Nounce": "8ac76d6efa5c5543d3"		// 用于获取 Token
    }
  }
  ```

#### 获取 AccessToken
- URI: `/wapi/auth/device/token`

- Method: `GET`

- Parameter:

  ```json
  {
    "Device": "W001_P13866886688",	// 设备ID, 可根据ID得到设备类型号
    "Secret": "a83b83ca9259c1sf8"		// 根据密钥类型对应的密钥与设备ID 和 Nounce hash 生成
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": {
      "Token": "8ac76d6efa5c5543d3",	// Access token
      "LifeTime": 1000,				// token 有效时间，单位秒
      "Bound": true,					// 设备是否已绑定，未绑定的设备，需要设备发绑定请求
      "BindGroup": 123,				// 设备未绑定时，发送绑定请求至此群组，大于0有效
      "Mqtt": [{					// 消息队列服务器信息
        "Host": "",				// optional, 主机名或IP
        "Type": "Tcp",			// 协议类型, "WebSocket", "Tcp"
        "Port": 1883,				// 端口号
        "Path": ""				// 访问路径
      }, {}]
    }
  }
  ```

### /wapi/auth/session

#### 用户登录

- REST Request
  ```http
  POST /wapi/auth/session
  ```
- Parameters:

  ```json
  {
    "Username": "admin",
    "Password": "fae4be532ce"		// 经MD5摘要后的16进制字串
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,				// optional, 错误码
    "ErrMsg": "Success",		// optional, 错误说明 
    "Result": {
      "Nick": "Deadpool",		// 用户昵称
      "Groups": [{			// 用户所属群组信息列表
        "Group": 234,			// 用户在群组的ID
        "UserRole": 123		// 用户在群内的角色号
      }, {}],
      "Mqtt": [{				// 消息队列服务器信息
        "Host": "",			// optional, 主机名或IP
        "Type": "WebSocket",	// 协议类型, "WebSocket", "Tcp"
        "Port": 15675,		// 端口号
        "Path": "/ws"			// 访问路径
      }, {}]
    }
  }
  ```

#### 用户登出

- REST Request:

  ```http
  DELETE /wapi/auth/session
  ```
- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": null
  }
  ```

### /wapi/auth/profile

#### 查询账号详细信息

- REST Request
  ```http
  GET /wapi/auth/profile
  ```

- RPC Request

  ```json
  {
    "Class": "IAuth",
    "Method": "getProfile",
    "Params": {}
  }
  ```

- Parameter:  `null`

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": {
      "Username": "user1",
      "Nick": "spock",
      "Groups": [{			// 用户所属群组信息列表
        "Group": 234,			// 用户在群组的ID
        "UserRole": 123		// 用户在群内的角色号
      }, {}]
    }
  }
  ```

#### 注册新账号（待定）

账号初始状态为禁用状态，需要再调用相应接口启用并激活账号。

- URI: `/wapi/auth/profile`

- Method: `POST`

- Parameter:

  ```json
  {
    "Username": "guest",	// 用户ID
    "Nick": "kerk"		// optional 用户昵称
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

#### 注销账号（待定）

从系统中删除账号信息，同时从所有关联群组中移除该账号。注意，只能删除操作者自己的账号。

- URI: `/wapi/auth/profile`

- Method: `DELETE`

- Parameter:

  ```json
  {
    "Username": "user1"	// 要移除的用户ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```


### /wapi/auth/active

更新用户状态（接口待定，暂未实现）

- URI: `/wapi/auth/active`
- Method: `PUT`

#### 激活用户（待定）

- Parameter:

  ```json
  {
    "Action": "Activate",		// 启用用户
    "Username": "admin"		// 用户ID
    "ActiveCode": "123456",    // 激活码
    "Password": "888888"		// 新密码明文
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

#### 重置用户（待定）

- Parameter:
  ```json
  {
    "Action": "Reset",		// 重置为未激活状态
    "Username": "admin"		// 用户ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

### /wapi/auth/password

#### 修改自身密码（待定）

- RPC Request

```json
{
  "Class": "IAuth",
  "Method": "setPassword",
  "Params": {}
}
```

- REST Request

```http
PUT /wapi/auth/password
```

- Parameter: 

```json
{
  "Old": "432cf3bea876c49a",     // 旧密码
  "New": "432cf37bc876c4c6"      // 新密码
}
```

- Response:

```json
{
  "ErrCode": 0
}
```

## 系统服务

### /wapi/sys/time

#### 获取服务器当前时间

用于校时

- REST Request
  ```http
  GET /wapi/sys/time
  ```

- RPC Request

  ```json
  {
    "Class": "IBox",
    "Method": "getTime",
    "Params": null
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": {
      "Now": 150277682347		// 服务器当前UTC时间
    }
  }
  ```


## 群组管理

### /wapi/group/nodes

#### 查询群组子节点树

- REST Request
  ```http
  GET /wapi/group/nodes
  ```

- RPC Request

  ```json
  {
    "Class": "IGroupManager",
    "Method": "list",
    "Params": {}
  }
  ```

- Parameter:

  ```json
  {
    "Root": 12345，	// 群组根节点ID
    "Deep": 1			// 递归深度，0表示不递归，只查询本身信息，-1表示全递归
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": {
      "Node": 111,
      "Name": "node111",
      "Memo": {},			// 附加信息
      "Children": [{
        "Node": 222,
        "Name": "node222",
        "Memo": {},
        "Children": null
      }, {}]
    }
  }
  ```

#### 创建群组节点

- REST Request
  ```http
  POST /wapi/group/nodes
  ```

- RPC Request

  ```json
  {
    "Class": "IGroupManager",
    "Method": "insert",
    "Params": {}
  }
  ```

- Parameter:

  ```json
  {
    "Parent": 111,
    "Name": "node111",	// optional
    "Memo": {				// optional 附加信息
      "Address":"杭州市西湖区",
      "Owner": [{			// 负责人信息
        "Name": "张三",
        "Phone": ["13966990066","0553-6238380"]
      }]
    }
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": {
      "Node": 999			// 新建的节点ID
    }
  }
  ```

#### 删除空群组节点

只能删除空群组节点，空节点是无任何下属的节点

- REST Request
  ```http
  DELETE /wapi/group/nodes
  ```

- RPC Request

  ```json
  {
    "Class": "IGroupManager",
    "Method": "remove",
    "Params": {}
  }
  ```

- Parameter:

  ```json
  {
    "Node": 111			// 删除的节点ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

#### 修改群组节点

- REST Request

  ```http
  PUT /wapi/group/nodes
  ```

- RPC Request

  ```json
  {
    "Class": "IGroupManager",
    "Method": "update",
    "Params": {}
  }
  ```

- Parameter:

  ```json
  {
    "Node": 222,
    "Parent": 111,
    "Name": "node111",	// optional
    "Memo": {}			// optional 附加信息
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

### /wapi/group/users

#### 查询组内用户列表

- REST Request

  ```http
  GET /wapi/group/users
  ```

- RPC Request

  ```json
  {
    "Class": "IUserManager",
    "Method": "list",
    "Params": {}
  }
  ```

- Parameter: 

  ```json
  {
    "Group": 2			// 所属群组
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": [{
      "Username": "user1",
      "Nick": "spock",
      "Role": 5				// 在群的角色
    }, {}]
  }
  ```

#### 创建组内用户

- REST Request

  ```http
  POST /wapi/group/users
  ```

- RPC Request

  ```json
  {
    "Class": "IUserManager",
    "Method": "insert",
    "Params": {}
  }
  ```

- Parameter:

  ```json
  {
    "Username": "guest",	// 用户ID
    "Password": "mima",	// 密码
    "Crypto": 0,			// 密码加密方式，0-不加密，1-加密方式1
    "Nick": "kerk",
    "Group": 2,			// 所属群组ID
    "Role": 2				// 在群的角色
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": {			// 当用户已经存在时，返回用户简要信息。（只在路桩控制系统中有效）
      "Groups": [{		// 用户所属群组信息列表
        "Group": 234,		// 用户在群组的ID
        "Name": "某警队"		// 群组名称
      }, {}]
    }
  }
  ```

#### 删除组内用户

先从群组中移除用户，如果移除后用户不属于任何群组，再从系统中删除用户账号。只能删除群管理员创建的账号。

- REST Request

  ```http
  DELETE /wapi/group/users
  ```

- RPC Request

  ```json
  {
    "Class": "IUserManager",
    "Method": "remove",
    "Params": {}
  }
  ```

- Parameter:

  ```json
  {
    "Group": 5,			// 群组ID
    "Username": "user1"	// 要移除的用户ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```


### /wapi/group/role

#### 更新组内用户角色

- URI: `/wapi/group/role`
- Method: `PUT`
- Parameter: 

```json
{
  "Username": "robin",	// 用户ID
  "Group": 5,			// 群组ID
  "Role": 2				// 角色编号
}
```
- Response:

```json
{
  "ErrCode": 0
}
```

### /wapi/group/password

#### 更新组内用户密码

不能修改自身密码，不能修改非群管理员创建的用户的密码

- URI: `/wapi/group/password`
- Method: `PUT`
- Parameter: 

```json
{
  "Group": 4,				// 用户所在群组
  "Username": "robin",		// 用户ID
  "Password": "password",	// 密码
  "Crypto": 0				// 密码加密方式，0-不加密，1-加密方式1
}
```

- Response:

```json
{
  "ErrCode": 0
}
```


## 设备管理

### /wapi/dev/devices

#### 查询设备列表

- URI: `/wapi/dev/devices`

- Method: `GET`

- Parameter:

  ```json
  {
    "Groups": [2],	// 设备所在群节点ID列表
    "Position": false	// 是否返回设置地理位置
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": [{
      "Group": 2345,				// 所属群组
    	"Device": "W001_P13866886688",	// 设备ID
      "Name": "豪华处理机",		// 设备名称
      "Model": "污水处理机",		// 设备型号，由厂家自定义
      "BindTime": 12345,			// 设备绑定时间(UTC)，单位秒
      "Memo": {},					// 附加信息
      "Position": [120.0, 30.5],	// (经度，纬度)
      "PositionTime": 1432344983	// 位置更新时间(UTC)，单位秒
    }, {}]
  }
  ```

#### 创建设备
- URI: `/wapi/dev/devices`

- Method: `POST`

- Parameter:

  ```json
  {
    "Group": 2345,					// 所属群组
    "Device": "W001_P13866886688",	// 设备ID
    "Name": "MachineI",				// 设备名称
    "Model": "污水处理机",				// 设备型号，由厂家自定义
    "Memo": {							// 附加信息
      "Owner":[{						// 责任人信息
        "Name":"张三",					// 姓名
        "Phone":"13966990066"			// 电话
      }, {}]
    }
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": null
  }
  ```

#### 删除设备
- URI: `/wapi/dev/devices`

- Method: `DELETE`

- Parameter:

  ```json
  {
    "Device": "0_P_15858275538"	// 设备ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": null
  }
  ```

#### 修改设备信息

- URI: `/wapi/dev/devices`

- Method: `PUT`

- Parameter:

  ```json
  {
    "Device": "W001_P13866886688",	// 设备ID
    "Name": "MachineI",				// 设备名称
    "Group": 2345,					// 所属群组
    "Model": "污水处理机",				// 设备型号，由厂家自定义
    "Memo": {}						// 附加信息
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": null
  }
  ```

##历史记录管理

### /wapi/record/tags (ITagRecord)

Tag 是指可以在时间轴上标记的一些对象，包含Event(事件报警)等，目前只有Event一种类型

注：协议更新了，PHP代码尚未同步更新 ---- 20171007

#### Tag上报

权限说明：上报者是Tag的拥有者或者是系统用户

- RPC Request

  ```json
  {
    "Class": "ITagRecord",
    "Method": "insert",
    "Params": {}
  }
  ```


- REST Request

  ```http
  POST /wapi/record/tags
  ```


- Parameter:

  ```json
  {
    "Device": "W001_P13866886688",	// Tag属于设备时有效
    "User": "user1",					// Tag属于用户时有效
    "Type": 1,				// Tag类型: 1-Event
    "Name":"Motion",			// Tag名称
    "Sensor": "1",			// 检测到事件的传感器标识号，意义同报警通道号
    "Action": 1,				// 动作类型: 0-stop, 1-start, 2-pulse
    "Time": [1000, 2000]		// 开始和结束时间，只有stop才需要带结束时间，单位为毫秒
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

#### Tag更新标志

- RPC Request

  ```json
  {
    "Class": "ITagRecord",
    "Method": "update",
    "Params": {}
  }
  ```


- REST Request

  ```http
  PUT /wapi/record/tags
  ```

- Parameter:

  ```json
  {
    "ID": 12345,			// 事件记录ID
    "Flag": 1				// 事件处理标志, 0-未处理，1-处理
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

#### Tag查询

- RPC Request

  ```json
  {
    "Class": "ITagRecord",
    "Method": "list",
    "Params": {}
  }
  ```


- REST Request

  ```http
  GET /wapi/record/tags
  ```

- Parameter:

  ```json
  {
    "Device": "W001_P13866886688",	// optional 查询指定设备
    "Groups": [],				// optional 查询群组列表内所有记录
    "Type": 1,				// optional Tag类型: 1-Event
    "Name": "Motion",			// optional 查询的Tag名称
    "Action": 1,				// optional 只查询指定的动作
    "Limit": 10,				// 分页控制，最大查询条数
    "Offset": 30				// 分页控制，从第几条开始查询
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": [{
      "ID": 123456,			// 记录ID号
      "Type": 1,				// Tag类型
      "Name": "Motion",		// TAG名称，UI上的事件原因据此显示
      "Action": 0,			// 最后收到的动作类型
      "Time": [1000, 2000],	// TAG起止时间
      "Device": "W001_P13866886688",	// TAG拥有者，属于设备时有效
      "User": "user1",				// TAG拥有者，属于用户时有效
      "Sensor": "1",			// TAG产生的传感器标识号或通道号
      "Flag": 0				// TAG处理标志, 0-未处理，1-处理
    }, {}]
  }
  ```

### /wapi/record/logs (ILogRecord)

#### 日志记录上报

权限说明：上报者是记录拥有者且隶属于群组

- RPC Request

  ```json
  {
    "Class": "ILogRecord",
    "Method": "insert",
    "Params": {}
  }
  ```


- REST Request
  ```http
  POST /wapi/record/logs
  ```

- Parameter:

  ```json
  {
    "Group": 5,					// 所属群组
    "Device": "W001_P13866886688",// optional 属于设备时有效
    "User": "user1",				// optional 属于用户时有效
    "Name": "Log1",		// 日志名称
    "Time": 1000,			// 日志产生时间，UTC
    "Brief": "xxx",		// optional 日志简要信息
    "Detail": {}			// optional 日志详细信息
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```


#### 日志记录查询

- RPC Request

  ```json
  {
    "Class": "ILogRecord",
    "Method": "list",
    "Params": {}
  }
  ```


- REST Request

  ```http
  GET /wapi/record/logs
  ```


- Parameter:

  ```json
  {
    "Group": 5,				// 日志所在群组
    "Name": "Log1",			// optional 查询的日志名称
    "Time": [1000, 2000],		// optional 起止时间(UTC)，单位为毫秒
    "User": "user1",			// optional 日志所属用户
    "Device": "devid",		// optional 日志所属设备
    "System": "system1"		// optional 日志所属系统用户
    "Limit": 10,				// optional 分页控制，最大查询条数
    "Offset": 30				// optional 分页控制，从第几条开始查询
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": [{
      "ID": 123456,			// 日志记录ID号
      "Group": 5,				// 日志所属群组
    	"Name": "Log1",			// 日志名称
      "User": "user1",		// 日志属于用户时有效
      "Device": "devid",		// 日志属于设备时有效
      "System": "system1",	// 日志属于系统用户时有效
      "Time": 1000,			// 日志产生时间，UTCMS
      "Brief": "xxx",			// 日志简要信息
      "Detail": {}			// 日志详细信息
    }, {}]
  }
  ```

## 元数据管理（配置管理）

### /wapi/meta/data

~~注：命名失误， 实际功能为群组配置管理，命名为`/wapi/meta/group`更贴切（20170923）~~

注：群组配置、设备配置统一使用此接口，故删除上条注解

群组配置、设备配置等，对外统一用`/wapi/meta/data`接口，根据传入的参数进行区分，后台分别操作不同的数据库表（group_meta、device_meta等）

#### 查询元数据

- REST Request
  ```http
  GET /wapi/meta/data
  ```

- Parameter:

  ```json
  {
    "Group": 4,			// 元数据所属群组
    "Key": "AlarmTips"	// optional 元数据键名，如，AlarmTips-报警提示音
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": [{				// 查询结果列表
      "Key": "AlarmTips",		// 元数据键名
      "Value": {}	 			// 元数据内容
    }, {}]
  }
  ```

#### 更新元数据

- REST Request
  ```http
  PUT /wapi/meta/data
  ```

- Parameter:

  ```json
  {
    "Group": 4,			// 元数据所属群组
    "Key": "AlarmTips"	// 元数据键名
    "Value": {}	 		// 元数据内容
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

### 元数据定义

#### AlarmTips（报警提示音）

```json
{
  "Total": 21,		// 曲目个数
  "Names": ["Music1", "Music2", null, "MusicN"]		// 曲目名称列表
}
```

## 消息管理 (IEventManager)

###消息订阅

- Rpc Request

```json
{
  "Class": "IEventManager",
  "Method": "attach",
  "Params": {}
}
```

- Params

```json
{
  "Topic": "command/control/12345"	// 消息主题
}
```

- Reply

```json
{
  "ErrCode": 0
}
```

###消息退订

- Rpc Request

```json
{
  "Class": "IEventManager",
  "Method": "detach",
  "Params": {}
}
```

- Params

```json
{
  "Topic": "command/control/12345"	// 消息主题
}
```

- Reply

```json
{
  "ErrCode": 0
}
```
###消息发布

- Rpc Request

```json
{
  "Class": "IEventManager",
  "Method": "notify",
  "Params": {}
}
```

- Params

```json
{
  "Topic": "command/control/12345",	// 消息主题
  "Info": {}          				// 消息内容
}
```

- Reply

```json
{
  "ErrCode": 0
}
```
###消息推送

事件消息推送，由服务器推送到客户端

- Rpc Push

```json
{
  "Class": "IEventManager",
  "Method": "push",
  "Params": {}
}
```

- Params

```json
{
  "Topic": "command/control/12345",	// 消息主题
  "Info": {}          				// 消息内容
}
```

- Reply

```json
{
  "ErrCode": 0
}
```


# 地理位置服务API

## 错误码

| 错误号  | 说明   |
| ---- | ---- |
|      |      |

## 位置管理
### /wapi/gps/info

设备位置、用户位置等，对外统一用`/wapi/gps/info`接口，根据传入的参数进行区分，后台分别操作不同的数据库表（gps_device、gps_user等）

#### 查询位置

- URI: `/wapi/gps/info`

- Method: `GET`

- Parameter:

  ```json
  {
    "Device": "W001_P13688660055"	// 设备ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": [{
      "Device": "W001_P13688660055",	// 设备ID
      "Position": [120.0, 30.5],	// (经度，纬度)
      "Time": 1432344983			// 位置更新时间(UTC)，单位秒
    }, {}]
  }
  ```

#### 更新位置

- URI: `/wapi/gps/info`

- Method: `PUT`

- Parameter:

  ```json
  {
    "Device": "W001_P13688660055",	// 设备ID
    "Position": [120.0, 30.5]		// (经度，纬度)
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```


### /wapi/gps/cities

#### 查询城市信息

- URI: `/wapi/gps/cities`

- Method: `GET`

- Parameter:

  ```json
  {
    "ID": 7869	// optional, 城市ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": [{
      "ID": 7869,		// 城市ID
    	"Name": "杭州"	// 城市名称
    }, {}]			// 其它城市信息
  }
  ```

#### 添加城市信息

- URI: `/wapi/gps/cities`

- Method: `POST`

- Parameter:

  ```json
  {
    "Name": "杭州"		// 城市名称
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
    "Result": {
      "ID": 7869		// 城市ID
    }
  }
  ```

#### 更新城市信息

- URI: `/wapi/gps/cities`

- Method: `PUT`

- Parameter:

  ```json
  {
    "ID": 7869,		// 城市ID
    "Name": "杭州"		// 城市名称
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

#### 删除城市信息

- URI: `/wapi/gps/cities`

- Method: `DELETE`

- Parameter:

  ```json
  {
    "ID": 7869		// 城市ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

### /wapi/gps/roads


#### 查询道路信息

- URI: `/wapi/gps/roads`

- Method: `GET`

- Parameter:

  ```json
  {
    "ID": 2232	// optional, 道路ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0,
    "Result": [{
      "ID": 2232				// 道路ID
    	"Name": "杭徽高速",		// 路桩控制手机号
    	"Endpoint": [{		// [起点城市, 终点城市]
        "ID": 7869,		// 城市ID
    	  "Name": "杭州"		// 城市名称
    	}, {}]			// 终点城市信息
    }, {}]			// 其它路桩信息
  }
  ```

#### 添加道路信息

- URI: `/wapi/gps/roads`

- Method: `POST`

- Parameter:

  ```json
  {
    "Name": "杭徽高速",		// 路桩控制手机号
    "Endpoint": [32, 33]		// [起点城市ID, 终点城市ID]
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

#### 更新道路信息

- URI: `/wapi/gps/roads`

- Method: `PUT`

- Parameter:

  ```json
  {
    "ID": 2232,				// 道路ID
    "Name": "杭徽高速",		// 路桩控制手机号
    "Endpoint": [32, 33]		// [起点城市ID, 终点城市ID]
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```

#### 删除道路信息

- URI: `/wapi/gps/roads`

- Method: `DELETE`

- Parameter:

  ```json
  {
    "ID": 2232		// 道路ID
  }
  ```

- Response:

  ```json
  {
    "ErrCode": 0
  }
  ```



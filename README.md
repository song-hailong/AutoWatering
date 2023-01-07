# Auto-Watering 自动浇水系统
使用ESP32-C3制作的可远程监控和控制的自动浇水系统。接入的平台为点灯科技。

开发环境为 CLion + Platform IO。

------

# 常见问题：

1. 继电器模块无法控制，不管输出高电平还是低电平，继电器都只有一个状态。给继电器供3.3V电源。
2. 湿度采集不准确，需按照视频重新计算参数。
3. 浇水后会有短暂的湿度采集出错，可能是供电问题，视频里所有模块都通过一个电源供电，可将水泵和其他模块分开供电。

---

## 一：项目文件介绍

### 1. **Hardware**

Hardware文件夹里是为合宙ESP32-C3开发板的资料。

### 2. **Software**

Software文件夹为程序源码。

### 3. AppInterface

AppInterface文件夹中提供了APP界面配置文件。

### 4. Docs

相关的参考文件，包括芯片的Datasheet等。

## 二：硬件说明

### 关于主控：

主控用的是合宙的ESP32-C3开发板，淘宝链接为 [合宙ESP32C3开发板](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.3ed92e8dKTe2kF&id=666579064570&_u=71vdumg4341e) ，选经典款。也可以使用其他ESP32-C3的开发板，但是烧入程序时需注释掉 platformio.ini 文件里的 `board_build.flash_mode = dio` 。

### 关于湿度检测模块：

使用电容式土壤湿度检测模块。

使用时最好将上半部分电路密封，防止水溅入，可使用胶枪密封。

![img](https://s2.loli.net/2023/01/07/btc4XxvM6NiLsTz.jpg)

## 三：程序说明

将main.cpp中的如下代码修改为自己的，设备密匙见下文。

<img src="https://s2.loli.net/2023/01/07/87GuI9lsEBdhXkP.png" alt="image-20230107212958489" style="zoom:75%;" />

## 四：APP配置

### 1. 下载并安装app

app下载链接：https://diandeng.tech/dev ，选择安卓或者苹果版本下载。

<img src="https://s2.loli.net/2023/01/07/BzD7MwijQgmUAr9.png" alt="image-20230107214244651" style="zoom: 33%;" />

安装好后注册一个账号并登录。

### 2. 在app中添加设备，获取Secret Key

进入App，点击右上角的“+”号，然后选择添加设备，点击选择Arduino -> WiFi接入-> 选择要接入的服务商 -> 复制申请到的Secret Key

<img src="https://s2.loli.net/2023/01/07/P8p61NKmslbS9vU.png" style="zoom:80%;" />

### 3. 配置界面

在设备列表页，点击刚才新建的设备图标，进入设备控制面板。首次进入设备控制面板，会弹出向导页，此处随便选。

再点击右上角的 **... > 界面配置**，粘贴[界面配置_v2.0.txt](3.AppInterface/界面配置_v2.0.txt)中的配置代码即可。

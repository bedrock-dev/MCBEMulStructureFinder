author: hhhxiao
bilibili:_hhhxiao
gihtub:https://github.com/hhhxiao/MCBEMulStructureFinder
使用方法
1. 准备一个1.16.4的BDS,一定不能有存档，在 server.properties 中设置种子
2. 把 mul_feature_finder.dll 和mul_finder.txt都移动到bedrock_server.exe所在文件夹
3. mul_finder.txt里面填写配置，一共五个数，分别是x最小值，x最大值，z最小值，z最大值，最大距离(zx是区块坐标)
4.用dll注入器把dll注入bedrock_server.exe并启动即可(方法和trapdoor (https://github.com/hhhxiao/TrapDoor)   相同)
5.登录服务器的瞬间BDS会自动开始计算并打印结果,理论上开始算后就能取消登录了
注
1.换种子的时候一定要删除存档！！！！！！！！！！！！！！
2.只能1.16.4使用，查找结果理论上全版本通用(只要不是太古老的版本)
import re
import time
import json

import requests as r

# 设定抢红包时间
theTime = '22:00'

# 时间转换
th = int(theTime.split(':')[0])
tm = int(theTime.split(':')[1])


# 获取时间是否在范围
def getTime():
    t = time.localtime()  # 当前时间的纪元值
    fmt = "%H %M %S"
    now = time.strftime(fmt, t)  # 将纪元值转化为包含时、分的字符串
    now = now.split(' ')  # 以空格切割，将时、分放入名为now的列表中
    hour = int(now[0])
    minute = int(now[1])
    sec = int(now[2])
    if hour == th and minute == tm and (3 >= sec >= 0):
        return True
    time.sleep(0.05)
    return False


header_getRedPak = {
    'Host': 'xy.disubang.com',
    'Connection': 'keep-alive',
    'User-Agent': 'Mozilla/5.0 (Linux; Android 9; MI 6 Build/PKQ1.190118.001; wv) AppleWebKit/537.36 (KHTML, '
                  'like Gecko) Version/4.0 Chrome/77.0.3865.120 MQQBrowser/6.2 TBS/045525 Mobile Safari/537.36 '
}


def getRedPack(my_id, late_sec):
    link_base = 'http://xy.disubang.com'
    link_geturl = link_base + '/mobile/Rob/underway/activity_id/59/user_id/' + my_id + '.html'
    link_getRedPak = link_base + '/mobile/Rob/prize.html?user_id=' + my_id + '&activity_id=59&__token__='

    req = r.get(link_geturl, headers=header_getRedPak)
    line = req.text
    cok = req.headers

    try:
        header_getRedPak['Cookie'] = cok["Set-Cookie"]
    except KeyError:
        pass

    matchObj = re.search(r'var token = "(.+)"', line, re.M | re.I)
    if matchObj:
        token = matchObj.group().split('"')[1]
        time.sleep(float(late_sec))
        rp_req = r.get(link_getRedPak + token, headers=header_getRedPak)
        # print(link_geturl)
        # print(link_getRedPak + token)
        try:
            data = json.loads(rp_req.text)
            if data['message'] == '请稍后再试':
                print('抢包失败了，程序可能失效！')
            if data['message'] == '活动未开始':
                print('活动未开始，程序可能失效！')
            if data['message'] == '你今日已经参加过活动啦':
                print('今日活动已参加！')
            if data['message'] == '参加活动成功等待开奖':
                print('正在查询中奖结果...')
                req = r.get(link_geturl, headers=header_getRedPak)
                text = req.text
                if '优惠券已发放至您的账户请注意查收' in text:
                    print('恭喜您！抢到红包了！')
                if '哎呀~红包与您擦肩而过' in text:
                    print('很遗憾没有抢到红包！')
                time.sleep(3)
        except json.decoder.JSONDecodeError:
            print('抢包失败!已被服务器暂时拦截，请稍后再试！')
        # 再次尝试等待0.5秒
        time.sleep(0.5)
    else:
        print("没有找到红包，正在重试!!")


if __name__ == '__main__':
    user_id = input('请输入迪速帮id号码,到22点将自动抢红包:\n')
    late_sec = input('需要延迟多少秒抢？（0为不延迟,不建议超过2秒,可能人多抢不到)\n请输入秒(支持小数如 1.25 ):\n')
    print('程序已就绪！')
    while True:
        while getTime():
            getRedPack(user_id, late_sec)

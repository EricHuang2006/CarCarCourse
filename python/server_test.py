import logging
import time
from BTinterface import BTInterface
from score import ScoreboardServer

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)
log = logging.getLogger(__name__)

TEAM_NAME = "ようこそ。Ave Mujica の世界へ"
SERVER_URL = "http://140.112.175.18:5000/"
BT_PORT = "COM11"  # ← 請填你的藍牙埠，例如 "COM11"

def add_score():
    log.info(f"讀取到 UID：{ss}")
    try:
        score, time_remaining = point.add_UID(ss)
        log.info(f"✅ 加分成功！這張得分: {score}，剩餘時間: {time_remaining:.2f} 秒")

        # 額外查詢目前總分
        current_score = point.get_current_score()
        if current_score is not None:
            log.info(f"📊 目前總分：{current_score}")
        else:
            log.warning("⚠️ 查詢目前總分失敗")
    except Exception as e:
        log.error(f"❌ 上傳失敗: {e}")

def server_main():
    point = ScoreboardServer(TEAM_NAME, SERVER_URL)
    BT = BTInterface(port=BT_PORT)

    try:
        while True:
            u = BT.get_UID()
            if u:
                ss = str(u).strip()
                if len(ss) == 8 and all(c in "0123456789ABCDEFabcdef" for c in ss):
                    log.info(f"讀取到 UID：{ss}")
                    try:
                        score, time_remaining = point.add_UID(ss)
                        log.info(f"✅ 加分成功！這張得分: {score}，剩餘時間: {time_remaining:.2f} 秒")

                        # 額外查詢目前總分
                        current_score = point.get_current_score()
                        if current_score is not None:
                            log.info(f"📊 目前總分：{current_score}")
                        else:
                            log.warning("⚠️ 查詢目前總分失敗")
                    except Exception as e:
                        log.error(f"❌ 上傳失敗: {e}")
                else:
                    log.warning(f"無效 UID：{ss}")
            time.sleep(0.2)
    except KeyboardInterrupt:
        log.info("🛑 中止程式")
        BT.bt.disconnect()

if __name__ == "__main__":
    server_main()

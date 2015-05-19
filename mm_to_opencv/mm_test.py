import sys
sys.path.insert(0, "/usr/local/google/home/dek/ImageJ")
import MMCorePy


if __name__ == '__main__':
    mmc = MMCorePy.CMMCore()
    mmc.enableStderrLog(True)
    mmc.enableDebugLog(True)

    mmc.loadSystemConfiguration("test.cfg")

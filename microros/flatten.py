import os
import shutil
import argparse

def flatten_one_dir(src):
    for file in os.listdir(src):
        if file == os.path.basename(src):
            tarDir = os.path.join(src, file)
            for tarFile in os.listdir(tarDir):
                if os.path.isdir(tarFile):
                    tarFileDir = os.path.join(tarDir, tarFile)
                    dstFileDir = os.path.join(src, tarFile)
                else:
                    tarFileDir = tarDir
                    dstFileDir = src
                shutil.copytree(tarFileDir, dstFileDir, dirs_exist_ok=True)
            shutil.rmtree(tarDir)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--path', type=str, default='', help='path to include')

    opt = parser.parse_args()

    PATH = opt.path

for file in sorted(os.listdir(PATH)):
    src = os.path.join(PATH, file)
    print(src)
    flatten_one_dir(src)
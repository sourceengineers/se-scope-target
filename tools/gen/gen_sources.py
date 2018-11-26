import sys
from shutil import copyfile
import os
import shutil, errno
from distutils.dir_util import copy_tree
from shutil import copyfile

#def copy(src, dst):
#    try:
#        shutil.copytree(src, dst)
#    except OSError as exc: # python >2.5
#        if exc.errno == errno.ENOTDIR:
#            shutil.copy(src, dst)
#        else: raise

def main():
    input_folder = os.path.abspath(sys.argv[1]);
    output_folder = os.path.abspath(sys.argv[2]);
    
    if os.path.lexists(input_folder) == False or os.path.lexists(input_folder) == False:
        print("Usage: python gen_sources.py PATH_TO_PROJECT PATH_TO_DESTINATION");
    
    copy_tree(os.path.join(input_folder, "src"), os.path.join(output_folder, "src"))
    copy_tree(os.path.join(input_folder, "3party/msgpackc/src"), os.path.join(output_folder, "src"))
    copy_tree(os.path.join(input_folder, "include/Scope"), os.path.join(output_folder, "include"))
    copy_tree(os.path.join(input_folder, "3party/msgpackc/include"), os.path.join(output_folder, "include"))
    copy_tree(os.path.join(input_folder, "doc"), os.path.join(output_folder, "doc"))
    copy_tree(os.path.join(input_folder, "tools/dev"), os.path.join(output_folder, "tools"))
        
    copyfile(os.path.join(input_folder, "README.md"), os.path.join(output_folder, "README.md"))

if __name__ == "__main__":
    main()

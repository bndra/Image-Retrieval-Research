#!/bin/bash
#SBATCH --job-name=COMPILE_SCALE_SPACE
#SBATCH --output=COMPILE_SCALE_SPACE.txt
#SBATCH --time=00:02:00

. /etc/profile.d/modules.sh

module load system/opencv/2.4.5
module load openmpi/2.1.2

g++ -o scale_space scale_space.cpp -I /opt/openmpi-2.1.2/include -L /opt/openmpi-2.1.2/lib -lmpi -lopencv_core -lopencv_highgui -lopencv_imgproc

#!/bin/bash
#SBATCH --job-name=RUN_SCALE_SPACE
#SBATCH --output=RUN_SCALE_SPACE.txt
#SBATCH --partition=compute
#SBATCH --mem=1gb
#SBATCH --nodes=3
#SBATCH --time=00:02:00

. /etc/profile.d/modules.sh

module load openmpi/2.1.2
module load system/opencv/2.4.5

/opt/openmpi-2.1.2/bin/mpirun ./scale_space

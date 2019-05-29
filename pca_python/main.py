'''
# PCA python (train)
# Author: Zhihui Lu
# Date: 2019/05/29
'''

import os
import numpy as np
import PCA_sklearn
import dataIO as io
from absl import flags, app


# flag
FLAGS = flags.FLAGS
flags.DEFINE_string("EUDT_txt", "", "EUDT list txt (include raw file path)")
flags.DEFINE_string("outdir", "", "output directory")
flags.DEFINE_string("svd_solver", "arpack", "type of svd_solver; [auto], [full], [arpack], [randomized]")
flags.DEFINE_list("image_size", [256, 256, 476], "image size")

def main(argv):
    # check folder
    if not (os.path.exists(FLAGS.outdir)):
        os.makedirs(FLAGS.outdir)

    # load data list
    data_list = io.load_list(FLAGS.EUDT_txt)

    # load data
    data = np.zeros((len(data_list), np.prod(FLAGS.image_size)))
    for i in range(len(data_list)):
        data[i, :] = io.read_raw(data_list[i], dtype='double')

    print("data shape: {}".format(data.shape))

    # Prepare for pca
    print('process pca')
    pca = PCA_sklearn.PCA(n_components=len(data_list) - 1, svd_solver=FLAGS.svd_solver)

    # Do pca and map to Principal component
    pca.fit(data)

    # mean_vector
    mean_vector = pca.mean_

    # components
    U = pca.components_

    # eigen_vector
    eigen_vector = pca.explained_variance_

    # output_result
    print("output result")

    ratio = pca.explained_variance_ratio_  # CCR
    ratio = np.cumsum(ratio)
    np.savetxt(FLAGS.outdir + '/CCR.txt', ratio, delimiter=',', fmt='%.6f')

    io.write_raw(mean_vector, FLAGS.outdir + '/mean.vect')  # mean

    io.write_raw(eigen_vector, FLAGS.outdir + '/eval.vect')  # eigen_vector
    np.savetxt(FLAGS.outdir + '/eval.txt', eigen_vector, delimiter=',', fmt='%.6f')

    for i in range(0, len(data_list) - 1):
        io.write_raw(U[i, :].copy(), FLAGS.outdir + '/vect_' + str(i).zfill(4) + '.vect')  # PCs


if __name__ == '__main__':
    app.run(main)

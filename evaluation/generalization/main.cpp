#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "dataIO.h"
#include "generalization.h"

typedef double T_pca;

int main(int argc, char *argv[]) {
	if (argc != 6) {
		std::cerr << "Usage: error" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* input information */
	const std::string LSDM_pathEvect = argv[1];				//LSDM_eigenvector_path
	const std::string LSDM_pathMean = argv[2];				//LSDM_mean_path
	const std::string test_eudt_list = argv[3];				//learning_eudt_path
	const std::string dirOut = argv[4];						//output directory

	const int LSDM_te = std::atoi(argv[5]);					//number of components
	int test_num;

	/* check folder */
	dataIO::check_folder(dirOut + "//");

	Eigen::Matrix<T_pca, Eigen::Dynamic, Eigen::Dynamic> LSDM_U;			//LSDM_eigenvector
	Eigen::Matrix<T_pca, Eigen::Dynamic, Eigen::Dynamic> LSDM_mean;			//LSDM_mean
	Eigen::Matrix<T_pca, Eigen::Dynamic, Eigen::Dynamic> test_eudt;			//test_eudt

	/* load LSDM */
	dataIO::load_matrix(LSDM_U, LSDM_pathEvect, LSDM_te);
	dataIO::load_matrix(LSDM_mean, LSDM_pathMean, 1);

	/* load test data */
	test_num = dataIO::count_number_of_text_lines(test_eudt_list);
	dataIO::load_matrix(test_eudt, test_eudt_list, test_num);

	/* generalization */
	evaluation::LSDM_Generalization(test_eudt, LSDM_mean, LSDM_U, LSDM_te, dirOut);

}
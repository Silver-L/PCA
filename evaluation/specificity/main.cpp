#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "dataIO.h"
#include "specificity.h"

typedef double T_pca;
typedef double T_data;

int main(int argc, char *argv[]) {
	if (argc != 8) {
		std::cerr << "Usage: error" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* input information */
	const std::string LSDM_pathEvect = argv[1];				//LSDM_eigenvector_path
	const std::string LSDM_pathEval = argv[2];				//LSDM_eigenvalue_path
	const std::string LSDM_pathMean = argv[3];				//LSDM_mean_path
	const std::string test_eudt_list = argv[4];			//test_eudt_path
	const std::string dirOut = argv[5];

	const int LSDM_te = std::atoi(argv[6]);
	const int num_gene = std::atoi(argv[7]);
	int test_num;

	/* check folder */
	dataIO::check_folder(dirOut + "//");

	Eigen::VectorXd LSDM_evalue;												//LSDM_eigenvalue
	Eigen::Matrix<T_pca, Eigen::Dynamic, Eigen::Dynamic> LSDM_U;				//LSDM_eigenvector
	Eigen::Matrix<T_pca, Eigen::Dynamic, Eigen::Dynamic> LSDM_mean;				//LSDM_mean
	Eigen::Matrix<T_data, Eigen::Dynamic, Eigen::Dynamic> test_eudt;			//test_eudt

	/* load LSDM */
	dataIO::load_matrix(LSDM_U, LSDM_pathEvect, LSDM_te);
	dataIO::load_matrix(LSDM_mean, LSDM_pathMean, 1);
	dataIO::load_vector(LSDM_evalue, LSDM_pathEval, LSDM_te);

	/* load test data */
	test_num = dataIO::count_number_of_text_lines(test_eudt_list);
	dataIO::load_matrix(test_eudt, test_eudt_list, test_num);

	evaluation::LSDM_Specificity(test_eudt, LSDM_mean, LSDM_U, LSDM_evalue, LSDM_te, num_gene, test_num, dirOut);

}
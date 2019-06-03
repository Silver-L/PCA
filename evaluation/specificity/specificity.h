#pragma once
#include <random>
#include "popcnt_JI.h"
#include "ItkImageIO.h"

namespace evaluation {

	/* LSDM_Specificity (test eudt should be the EUDT file) */
	template<class T, class T2, class T3, class T4>
	void LSDM_Specificity(T &test_eudt, T2 &LSDM_mean, T3 &LSDM_U, T4 &LSDM_evalue, int LSDM_te, int ngene, int test_num, std::string dirOut) {

		std::stringstream csv_index;
		csv_index << LSDM_te;
		std::string i_fn = dirOut + "/" + "LSDM_Specificity_" + csv_index.str() +".csv";
		std::ofstream out(i_fn, std::ios::out);

		int seed_gen = -1;
		std::default_random_engine engine(seed_gen);
		std::normal_distribution<> dist(0.0, 1.0);

		/* element count of package */
		int ele_num = popcnt::element_count(int(test_eudt.rows()));

		/* pack test_data */
		std::vector<std::vector<unsigned char>> test_label;
		std::vector<unsigned long long*> test_data(test_num);
		for (int i = 0; i < test_eudt.cols(); i++) {

			/* change test_eudt to label */
			std::vector<unsigned char> test_label_single(test_eudt.rows());
			for (int j = 0; j < test_eudt.rows(); j++) {
				if (test_eudt(j, i) <= 0.0) {
					test_label_single[j] = 1;
				}
				else test_label_single[j] = 0;
			}
			test_label.push_back(test_label_single);

			unsigned char *A = &test_label[i][0];
			unsigned long long *P = new unsigned long long[test_eudt.rows()];
			popcnt::pack(A, P, (int)test_eudt.rows());
			test_data[i] = P;
		}

		double specificity = 0.0;
		std::cout << "LSDM_te= " << LSDM_te << std::endl;

		/* Arbitrarily_Shaped_Object */
		for (int j = 0; j < ngene; j++) {
			std::cout << "sample: " << j + 1 << std::endl;
			Eigen::VectorXd LSDM_score(LSDM_te);
			for (int t = 0; t < LSDM_te; t++) {
				LSDM_score(t) = sqrt(LSDM_evalue(t))*dist(engine);
			}

			Eigen::VectorXd LSDM_out = LSDM_U * LSDM_score + LSDM_mean;
			std::vector<unsigned char>LSDM_out_label(test_eudt.rows(), 0);
			for (int i = 0; i < LSDM_out_label.size(); i++) {
				if (LSDM_out(i) <= 0.0) LSDM_out_label[i] = 1;
				else LSDM_out_label[i] = 0;
			}

			/* calculate J.I. */
			unsigned char *B = &LSDM_out_label[0];
			unsigned long long *Q = new unsigned long long[test_eudt.rows()];
			popcnt::pack(B, Q, (int)test_eudt.rows());

			double JI;
			double case_max_ji = 0.0;
			for (int i = 0; i < test_eudt.cols(); i++) {
				JI = popcnt::JI(Q, test_data[i], ele_num);

				if (JI > case_max_ji) case_max_ji = JI;
			}

			/* memory release */
			delete[]Q;

			specificity += case_max_ji;

			out << case_max_ji << std::endl;

			/* visual */
		//	ImageIO<3> imageio;
		//	imageio.SetSize(0, 256);
		//	imageio.SetSize(1, 256);
		//	imageio.SetSize(2, 476);
		//	imageio.SetSpacing(0, 1.25);
		//	imageio.SetSpacing(1, 1.25);
		//	imageio.SetSpacing(2, 1.25);
		//	imageio.SetIndex(0, 0);
		//	imageio.SetIndex(1, 0);
		//	imageio.SetIndex(2, 0);
		//	dataIO::check_folder(dirOut + "//specificity//");
		//	imageio.Write(LSDM_out_label, dirOut + "//specificity//generate_" + std::to_string(j) + ".mhd", true);
		}

		specificity /= (double)ngene;
		std::cout << "specificity= " << specificity << std::endl;
		out << "specificity:," << specificity << std::endl;
		out.close();
	}
}
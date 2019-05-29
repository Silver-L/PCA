#pragma once
#include "popcnt_JI.h"
#include "ItkImageIO.h"
#include "dataIO.h"

typedef double T_eudt;
typedef unsigned char T_label;

namespace evaluation {

	/* LSDM_Generalization */
	template<class T, class T2, class T3/*, class T4*/>
	void LSDM_Generalization(T &test_eudt, T2 &LSDM_mean, T3 &LSDM_U, /*T4 &weight, */int LSDM_te, std::string dirOut) {

		std::stringstream csv_index;
		csv_index << LSDM_te;
		std::string i_fn = dirOut + "/"  + "LSDM_Generalization_" + csv_index.str() +".csv";
		std::ofstream out(i_fn, std::ios::out);

		/* element count of package */
		int ele_num = popcnt::element_count(int(LSDM_mean.rows()));

		double generalization = 0.0;

		for (int t = 0; t < test_eudt.cols(); t++) {

			///* WPCA */
			//for (int i = 0; i < test_eudt.rows(); i++) {
			//	test_eudt(i, t) = weight(i, 0)*test_eudt(i, t);
			//}

			Eigen::VectorXd LSDM_score = LSDM_U.transpose()*(test_eudt.col(t) - LSDM_mean);

			Eigen::VectorXd LSDM_out = LSDM_U * LSDM_score + LSDM_mean;

			/* label */
			std::vector<unsigned char> LSDM_out_label(test_eudt.rows(), 0);
			for (int i = 0; i < test_eudt.rows(); i++) {
				if (LSDM_out(i) <= 0.0) LSDM_out_label[i] = 1;
				else LSDM_out_label[i] = 0;
			}

			/* ground truth */
			std::vector<unsigned char> ground_truth(test_eudt.rows(), 0);
			for (int i = 0; i < test_eudt.rows(); i++) {
				if (test_eudt(i, t) <= 0.0) ground_truth[i] = 1;
				else ground_truth[i] = 0;
			}

			unsigned char *A = &ground_truth[0];
			unsigned long long *P = new unsigned long long[test_eudt.rows()];
			unsigned char *B = &LSDM_out_label[0];
			unsigned long long *Q = new unsigned long long[test_eudt.rows()];
			popcnt::pack(A, P, int(test_eudt.rows()));
			popcnt::pack(B, Q, int(test_eudt.rows()));

			double JI = 0.0;
			JI = popcnt::JI(P, Q, ele_num);

			/* memory release */
			delete[]P;
			delete[]Q;

			generalization += JI;

			out << JI << std::endl;

			/* visual */
			//ImageIO<3> imageio;
			//imageio.SetSize(0, 256);
			//imageio.SetSize(1, 256);
			//imageio.SetSize(2, 476);
			//imageio.SetSpacing(0, 1.25);
			//imageio.SetSpacing(1, 1.25);
			//imageio.SetSpacing(2, 1.25);
			//imageio.SetIndex(0, 0);
			//imageio.SetIndex(1, 0);
			//imageio.SetIndex(2, 0);
			//dataIO::check_folder(dirOut + "//generalization//");
			//imageio.Write(LSDM_out_label, dirOut + "//generalization//" + "recon_" + std::to_string(t) + ".mhd", true);

		}

		generalization /= (double)test_eudt.cols();
		std::cout << generalization << std::endl;
		out << "generalization:," << generalization << std::endl;
		out.close();
	}
}
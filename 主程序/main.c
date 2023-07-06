#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#define Data 380
#define TestData 126
#define In 13
#define Out 1
double d_in[Data][In];
double d_out[Data][Out];
double D_in[Data][4];
double t_in[TestData][In];
double T_in[TestData][4];
double t_out[TestData][Out];
double d_inAverage[In];
double d_outAverage[Out];
double Maxin[In], Minin[In];
double Maxout[Out], Minout[Out];
int TrainCount = 10000;
double w[In + 1];
double wP[5];
double alfa = 0.04;//学习率
double RMSE = 0;
void ReadData()
{
	FILE* fp1, * fp2;
	char ch;
	if ((fp1 = fopen("in.txt", "rb")) == NULL) {
		printf("不能打开in.txt文件\n");
		exit(0);
	}
	for (int i = 0; i < Data; i++) {
		for (int j = 0; j < In; j++) {
			if (j != 0) {
				fscanf(fp1, "%c", &ch);
			}
			fscanf(fp1, "%lf", &d_in[i][j]);
		}
	}
	fclose(fp1);
	if ((fp2 = fopen("out.txt", "rb")) == NULL) {
		printf("不能打开out.txt文件\n");
		exit(0);
	}
	for (int i = 0; i < Data; i++) {
		for (int j = 0; j < Out; j++) {

			fscanf(fp2, "%lf", &d_out[i][j]);
		}
	}
	fclose(fp1);
}
void GuiYiHua() //归一化,初始化
{
	for (int i = 0; i < In; i++) {
		Minin[i] = Maxin[i] = d_in[0][0];
		for (int j = 0; j < Data; j++) {
			Maxin[i] = Maxin[i] > d_in[j][i] ? Maxin[i] : d_in[j][i];
			Minin[i] = Minin[i] < d_in[j][i] ? Minin[i] : d_in[j][i];
		}
	}
	for (int i = 0; i < Out; i++) {
		Minout[i] = Maxout[i] = d_out[0][i];
		for (int j = 0; j < Data; j++) {
			Maxout[i] = Maxout[i] > d_out[j][i] ? Maxout[i] : d_out[j][i];
			Minout[i] = Minout[i] < d_out[j][i] ? Minout[i] : d_out[j][i];
		}
	}
	for (int i = 0; i < In; i++)
		for (int j = 0; j < Data; j++)
			d_in[j][i] = (d_in[j][i] - Minin[i]) / (Maxin[i] - Minin[i]);
	for (int i = 0; i < Out; i++)
		for (int j = 0; j < Data; j++)
			d_out[j][i] = (d_out[j][i] - Minout[i]) / (Maxout[i] - Minout[i]);
	for (int i = 0; i < In; i++)
		d_inAverage[i] = 0;
	for (int i = 0; i < Out; i++)
		d_outAverage[i] = 0;
	for (int i = 0; i < In; i++) {
		for (int j = 0; j < Data; j++) {
			d_inAverage[i] = d_inAverage[i] + d_in[j][i];
		}
		d_inAverage[i] /= Data;
	}
	for (int i = 0; i < Out; i++) {
		for (int j = 0; j < Data; j++) {
			d_outAverage[i] = d_outAverage[i] + d_out[j][i];
		}
		d_outAverage[i] /= Data;
	}
	for (int i = 0; i < In; i++)
		w[i] = 0.5;
	for (int i = 0; i < 5; i++)
		wP[i] = 0.5;
}
void Pearson() //皮尔逊相关系数
{
	double r[In];
	double sum1 = 0, sum2 = 0, sum3 = 0;
	for (int i = 0; i < In; i++) {
		sum1 = sum2 = sum3 = 0;
		for (int j = 0; j < Data; j++) {
			sum1 += (d_in[j][i] - d_inAverage[i]) * (d_out[j][Out] - d_outAverage[i]);
			sum2 += (d_in[j][i] - d_inAverage[i]) * (d_in[j][i] - d_inAverage[i]);
			sum3 += (d_out[j][Out] - d_outAverage[i]) * (d_out[j][Out] - d_outAverage[i]);
		}
		r[i] = sum1 / (sqrt(sum2) * sqrt(sum3));
		printf("%lf\n", r[i]);
	}
	printf("\n");
	for (int j = 0; j < Data; j++) {
		D_in[j][0] = d_in[j][0];
		D_in[j][1] = d_in[j][2];
		D_in[j][2] = d_in[j][3];
		D_in[j][3] = d_in[j][9];
	}
}
void Train()
{
	double sum1, sum2;
	for (int i = 0; i < TrainCount; i++) {
		for (int m = 0; m < In + 1; m++) {
			sum2 = 0;
			for (int j = 0; j < Data; j++) {
				sum1 = w[0];
				for (int k = 0; k < In; k++) {
					sum1 += w[k + 1] * d_in[j][k];
				}
				if (m == 0) sum2 += sum1 - d_out[j][Out];
				else
					sum2 += (sum1 - d_out[j][Out]) * d_in[j][m - 1];
			}
			w[m] = w[m] - alfa / Data * sum2;
		}

	}
	for (int m = 0; m < In + 1; m++)
		printf("%lf\n", w[m]);
}
void TrainP()
{
	double sum1, sum2;
	for (int i = 0; i < TrainCount; i++) {
		for (int m = 0; m < 4 + 1; m++) {
			sum2 = 0;
			for (int j = 0; j < Data; j++) {
				sum1 = wP[0];
				for (int k = 0; k < 4; k++) {
					sum1 += wP[k + 1] * D_in[j][k];
				}
				if (m == 0) sum2 += sum1 - d_out[j][Out];
				else
					sum2 += (sum1 - d_out[j][Out]) * D_in[j][m - 1];
			}
			wP[m] = wP[m] - alfa / Data * sum2;
		}

	}
	for (int m = 0; m < 4 + 1; m++)
		printf("%lf\n", wP[m]);
}

void Test()
{
	FILE* fp;
	char ch;
	//打开保存测试数据集的文件
	if ((fp = fopen("test.txt", "rb")) == NULL) {
		printf("不能打开test.txt文件\n");
		exit(0);
	}
	//输入测试数据集
	for (int i = 0; i < TestData; i++) {
		for (int j = 0; j < In + Out; j++) {
			if (j != 0) {
				fscanf(fp, "%c", &ch);
			}
			if (j < In) {
				fscanf(fp, "%lf", &t_in[i][j]);
			}
			else {
				fscanf(fp, "%lf", &t_out[i][j - In]);
			}
		}
	}
	fclose(fp);
	//测试数据归一化处理
	for (int i = 0; i < In; i++) {
		Minin[i] = Maxin[i] = t_in[0][0];
		for (int j = 0; j < TestData; j++) {
			Maxin[i] = Maxin[i] > t_in[j][i] ? Maxin[i] : t_in[j][i];
			Minin[i] = Minin[i] < t_in[j][i] ? Minin[i] : t_in[j][i];
		}
	}
	for (int i = 0; i < Out; i++) {
		Minout[i] = Maxout[i] = t_out[0][i];
		for (int j = 0; j < TestData; j++) {
			Maxout[i] = Maxout[i] > t_out[j][i] ? Maxout[i] : t_out[j][i];
			Minout[i] = Minout[i] < t_out[j][i] ? Minout[i] : t_out[j][i];
		}
	}
	for (int i = 0; i < In; i++) {
		for (int j = 0; j < TestData; j++) {
			t_in[j][i] = (t_in[j][i] - Minin[i]) / (Maxin[i] - Minin[i]);

		}
	}
	for (int i = 0; i < Out; i++) {
		for (int j = 0; j < TestData; j++) {
			t_out[j][i] = (t_out[j][i] - Minout[i]) / (Maxout[i] - Minout[i]);
		}
	}
	double sum1, sum2 = 0;
	for (int j = 0; j < TestData; j++) {
		sum1 = w[0];
		for (int k = 0; k < In; k++) {
			sum1 += w[k + 1] * d_in[j][k];
		}
		sum2 += (sum1 - t_out[j][Out]) * (sum1 - t_out[j][Out]);
	}
	printf("%lf\n", sum2);
	RMSE = sqrt(sum2 / TestData);
	printf("%lf", RMSE);
}
void TestP()
{
	FILE* fp;
	char ch;
	//打开保存测试数据集的文件
	if ((fp = fopen("test.txt", "rb")) == NULL) {
		printf("不能打开test.txt文件\n");
		exit(0);
	}
	//输入测试数据集
	for (int i = 0; i < TestData; i++) {
		for (int j = 0; j < In + Out; j++) {
			if (j != 0) {
				fscanf(fp, "%c", &ch);
			}
			if (j < In) {
				fscanf(fp, "%lf", &t_in[i][j]);
			}
			else {
				fscanf(fp, "%lf", &t_out[i][j - In]);
			}
		}
	}
	fclose(fp);
	//测试数据归一化处理
	for (int i = 0; i < In; i++) {
		Minin[i] = Maxin[i] = t_in[0][0];
		for (int j = 0; j < TestData; j++) {
			Maxin[i] = Maxin[i] > t_in[j][i] ? Maxin[i] : t_in[j][i];
			Minin[i] = Minin[i] < t_in[j][i] ? Minin[i] : t_in[j][i];
		}
	}
	for (int i = 0; i < Out; i++) {
		Minout[i] = Maxout[i] = t_out[0][i];
		for (int j = 0; j < TestData; j++) {
			Maxout[i] = Maxout[i] > t_out[j][i] ? Maxout[i] : t_out[j][i];
			Minout[i] = Minout[i] < t_out[j][i] ? Minout[i] : t_out[j][i];
		}
	}
	for (int i = 0; i < In; i++) {
		for (int j = 0; j < TestData; j++) {
			t_in[j][i] = (t_in[j][i] - Minin[i]) / (Maxin[i] - Minin[i]);

		}
	}
	for (int i = 0; i < Out; i++) {
		for (int j = 0; j < TestData; j++) {
			t_out[j][i] = (t_out[j][i] - Minout[i]) / (Maxout[i] - Minout[i]);
		}
	}
	double sum1, sum2 = 0;
	for (int j = 0; j < TestData; j++) {
		sum1 = wP[0];
		for (int k = 0; k < 4; k++) {
			sum1 += wP[k + 1] * D_in[j][k];
		}
		sum2 += (sum1 - t_out[j][Out]) * (sum1 - t_out[j][Out]);
	}
	RMSE = sqrt(sum2 / TestData);
	printf("误差：%lf", RMSE);
}
int main()
{
	ReadData();
	GuiYiHua();
	Pearson();
	TrainP();
	TestP();
	return 0;
}


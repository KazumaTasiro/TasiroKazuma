#include "Matrix4.h"
#include "Affin.h"


Matrix4::Matrix4() {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = 0.0f;
		}
	}

}

Matrix4::Matrix4(float num) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = num;
		}
	}

}

// 成分を指定しての生成
Matrix4::Matrix4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33) {
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}


Vector3 Matrix4::transform(const Vector3& v, const Matrix4& m) {

	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w,
	};

	return result;
}

void Matrix4::MatrixInverse(Matrix4& pOut, Matrix4& pM)
{
	Matrix4 mat;
	int i, j, loop;
	double fDat, fDat2;
	double mat_8x4[4][8];
	int flag;
	float* pF;
	double* pD;

	//8 x 4行列に値を入れる
	for (i = 0; i < 4; i++) {
		pF = pM.m[i];
		for (j = 0; j < 4; j++, pF++) mat_8x4[i][j] = (double)(*pF);
		pD = mat_8x4[i] + 4;
		for (j = 0; j < 4; j++) {
			if (i == j)   *pD = 1.0;
			else         *pD = 0.0;
			pD++;
		}
	}

	flag = 1;
	for (loop = 0; loop < 4; loop++) {
		fDat = mat_8x4[loop][loop];
		if (fDat != 1.0) {
			if (fDat == 0.0) {
				for (i = loop + 1; i < 4; i++) {
					fDat = mat_8x4[i][loop];
					if (fDat != 0.0) break;
				}
				if (i >= 4) {
					flag = 0;
					break;
				}
				//行を入れ替える
				for (j = 0; j < 8; j++) {
					fDat = mat_8x4[i][j];
					mat_8x4[i][j] = mat_8x4[loop][j];
					mat_8x4[loop][j] = fDat;
				}
				fDat = mat_8x4[loop][loop];
			}

			for (i = 0; i < 8; i++) mat_8x4[loop][i] /= fDat;
		}
		for (i = 0; i < 4; i++) {
			if (i != loop) {
				fDat = mat_8x4[i][loop];
				if (fDat != 0.0f) {
					//mat[i][loop]をmat[loop]の行にかけて
					//(mat[j] - mat[loop] * fDat)を計算
					for (j = 0; j < 8; j++) {
						fDat2 = mat_8x4[loop][j] * fDat;
						mat_8x4[i][j] -= fDat2;
					}
				}
			}
		}
	}

	if (flag) {
		for (i = 0; i < 4; i++) {
			pF = mat.m[i];
			pD = mat_8x4[i] + 4;
			for (j = 0; j < 4; j++) {
				*pF = (float)(*pD);
				pF++;
				pD++;
			}
		}
	}
	else {
		//単位行列を求める
		mat = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
		};
	}

	pOut = mat;
}

Matrix4 Matrix4::MakeInverse(const Matrix4* mat)
{
	assert(mat);

	//掃き出し法を行う行列
	float sweep[4][8]{};
	//定数倍用
	float constTimes = 0.0f;
	//許容する誤差
	float MAX_ERR = 1e-10f;
	//戻り値用
	Matrix4 retMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//weepの左側に逆行列を求める行列をセット
			sweep[i][j] = mat->m[i][j];

			//sweepの右側に単位行列をセット
			sweep[i][4 + j] = MakeIdentity().m[i][j];
		}
	}

	//全ての列の対角成分に対する繰り返し
	for (int i = 0; i < 4; i++)
	{
		//最大の絶対値を注目対角成分の絶対値と仮定
		float max = static_cast<float>(fabs(sweep[i][i]));
		int maxIndex = i;

		//i列目が最大の絶対値となる行を探す
		for (int j = i + 1; j < 4; j++)
		{
			if (fabs(sweep[j][i]) > max)
			{

				max = static_cast<float>(fabs(sweep[j][i]));
				maxIndex = j;
			}
		}

		if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
		{
			//逆行列は求められない
			return MakeIdentity();
		}

		//操作(1):i行目とmaxIndex行目を入れ替える
		if (i != maxIndex)
		{
			for (int j = 0; j < 8; j++)
			{
				float tmp = sweep[maxIndex][j];
				sweep[maxIndex][j] = sweep[i][j];
				sweep[i][j] = tmp;
			}
		}

		//sweep[i][i]に掛けると1になる値を求める
		constTimes = 1 / sweep[i][i];

		//操作(2):p行目をa倍する
		for (int j = 0; j < 8; j++)
		{
			//これによりsweep[i][i]が1になる
			sweep[i][j] *= constTimes;
		}

		//操作(3)によりi行目以外の行のi列目を0にする
		for (int j = 0; j < 4; j++)
		{
			if (j == i)
			{
				//i行目はそのまま
				continue;
			}

			//i行目に掛ける値を求める
			constTimes = -sweep[j][i];

			for (int k = 0; k < 8; k++)
			{
				//j行目にi行目をa倍した行を足す
				//これによりsweep[j][i]が0になる
				sweep[j][k] += sweep[i][k] * constTimes;
			}
		}
	}

	//sweepの右半分がmatの逆行列
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			retMat.m[i][j] = sweep[i][4 + j];
		}
	}

	return retMat;
}

Matrix4 Matrix4::Matrix4Identity()
{
	Matrix4 mat;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat.m[i][j] = 0.0f;
		}
	}
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	return mat;
}

void Matrix4::MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix)
{

	float width = 1.0f / (right - left);
	float height = 1.0f / (top - bottom);
	float range = 1.0f / (far_ - near_);

	matrix.m[0][0] = width * 2;

	matrix.m[1][1] = height * 2;

	matrix.m[2][2] = range;

	matrix.m[3][0] = -(left + right) * width;
	matrix.m[3][1] = -(top + bottom) * height;
	matrix.m[3][2] = range * -near_;
	matrix.m[3][3] = 1.0f;

	matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
		matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
		matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][3] = 0.0f;
}

void Matrix4::MakePerspectiveL(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix)
{

	float sinFov = 0.0f;
	float cosFov = 0.0f;
	Affin::SinCos(sinFov, cosFov, 0.5f * fovAngleY);

	float range = far_ / (far_ - near_);
	float height = cosFov / sinFov;

	matrix.m[0][0] = height / aspect;

	matrix.m[1][1] = cosFov / sinFov;

	matrix.m[2][2] = range;
	matrix.m[2][3] = 1.0f;

	matrix.m[3][2] = -range * near_;

	matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
		matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
		matrix.m[2][0] = matrix.m[2][1] =
		matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][3] = 0.0f;
}

void Matrix4::MakeLookL(const Vector3& eye, const Vector3& target, const Vector3& up, Matrix4& mat)
{
	Vector3 zVec = target - eye;
	zVec.nomalize();

	Vector3 xVec = up.cross(zVec);
	xVec.nomalize();

	Vector3 yVec = zVec.cross(xVec);
	yVec.nomalize();

	mat.m[0][0] = xVec.x;
	mat.m[0][1] = xVec.y;
	mat.m[0][2] = xVec.z;
	mat.m[1][0] = yVec.x;
	mat.m[1][1] = yVec.y;
	mat.m[1][2] = yVec.z;
	mat.m[2][0] = zVec.x;
	mat.m[2][1] = zVec.y;
	mat.m[2][2] = zVec.z;
	mat.m[3][0] = eye.x;
	mat.m[3][1] = eye.y;
	mat.m[3][2] = eye.z;
}


//	--		 --			--		 --
//	| a b c d |			| A B C D |
//	| e f g h |	   X	| E F G H |
//	| i j k l |			| I J K L |
//	| m n o p |			| M N O P |
//	--		 --			--		 --



Matrix4& operator*=(Matrix4& m1, const Matrix4& m2) {

	Matrix4 result{ 0 };

	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			result.m[i][k] =
				m1.m[i][0] * m2.m[0][k] +
				m1.m[i][1] * m2.m[1][k] +
				m1.m[i][2] * m2.m[2][k] +
				m1.m[i][3] * m2.m[3][k];
		}
	}
	m1 = result;
	return m1;
}



// 2項演算子オーバーロード ( 行列と行列の積 )
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2) {
	Matrix4 result = m1;

	return result *= m2;
}
//2項演算子オーバーロード ( ベクトルと行列の積 )
const Vector3 operator*(const Vector3& v, const Matrix4& m2) {
	Matrix4 mat = Affin::matUnit();
	Vector3 result = mat.transform(v, m2);
	return result;
}

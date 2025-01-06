#include <navigation.h>

/**
 * @brief Construct a new Navigation:: Navigation object
 *
 * @param hal
 * @param plane
 */
Navigation::Navigation(HAL* hal, Plane* plane) : kalman(n, m, get_a(), get_b(), get_q())
{
	_hal = hal;
	_plane = plane;
}

Eigen::MatrixXf Navigation::get_a()
{
	float predict_dt = 0.01;
	Eigen::MatrixXf A(n, n);
	A << 1, 0, 0, predict_dt, 0, 0,
		 0, 1, 0, 0, predict_dt, 0,
		 0, 0, 1, 0, 0, predict_dt,
		 0, 0, 0, 1, 0, 0,
		 0, 0, 0, 0, 1, 0,
		 0, 0, 0, 0, 0, 1;
	return A;
}

Eigen::MatrixXf Navigation::get_b()
{
	float predict_dt = 0.01;
	Eigen::MatrixXf B(n, m);
	B << 0.5*predict_dt*predict_dt, 0, 0,
			  0, 0.5*predict_dt*predict_dt, 0,
			  0, 0, 0.5*predict_dt*predict_dt,
			  predict_dt, 0, 0,
			  0, predict_dt, 0,
			  0, 0, predict_dt;
	return B;
}

Eigen::MatrixXf Navigation::get_q()
{
	Eigen::DiagonalMatrix<float, n> Q(1, 1, 1, 1, 1, 1);
	return Q;
}

/**
 * @brief Update navigation
 *
 */
void Navigation::execute()
{
	printf("a");
	if (check_new_imu_data()) {
		predict_imu();
	}
	printf("b");

	if (check_new_gnss_data())
	{
//		update_gps();
	}

	if (check_new_baro_data())
	{
		printf("allahu");
		update_baro();
	}

	printf("c");

	update_plane();
}

void Navigation::predict_imu()
{
	read_imu();

	Eigen::VectorXf u(m);
	u << _plane->nav_acc_north,
		 _plane->nav_acc_east,
		 _plane->nav_acc_down;

	kalman.predict(u);

//	if (fabs(_plane->nav_pos_north) > 0.5)
//	{
//		kalman.reset();
//	}
}

void Navigation::update_gps()
{
	read_gnss();

	Eigen::VectorXf y(2);
	y << gnss_n,
		 gnss_e;

	Eigen::MatrixXf H(2, n);
	H << 1, 0, 0, 0, 0, 0,
		 0, 1, 0, 0, 0, 0;

	Eigen::DiagonalMatrix<float, 2> R(1, 1);

	kalman.update(R, H, y);
}

void Navigation::update_baro()
{
	Eigen::VectorXf y(1);
	y << _plane->baro_alt;
	last_baro_timestamp = _plane->baro_timestamp;

	Eigen::MatrixXf H(1, n);
	H << 0, 0, 1, 0, 0, 0;

	Eigen::DiagonalMatrix<float, 1> R(1);

	kalman.update(R, H, y);
}

void Navigation::update_plane()
{
	Eigen::MatrixXf est = kalman.get_estimate();
	_plane->nav_pos_north = est(0, 0);
	_plane->nav_pos_east = est(1, 0);
	_plane->nav_pos_down = est(2, 0);
	_plane->nav_vel_north = est(3, 0);
	_plane->nav_vel_east = est(4, 0);
	_plane->nav_vel_down = est(5, 0);
}

// Rotate inertial frame to ECF
void Navigation::read_imu()
{
	// Get IMU data
	Eigen::Vector3f acc_inertial(_plane->imu_ax, _plane->imu_ay, _plane->imu_az);
	last_imu_timestamp = _plane->imu_timestamp;

	// Get quaternion from AHRS
	Eigen::Quaternionf q(_plane->ahrs_q0, _plane->ahrs_q1, _plane->ahrs_q2, _plane->ahrs_q3);

	// Convert from inertial frame to ECF
	Eigen::Vector3f acc_world = q * acc_inertial * g;
	acc_world(2) += g; // Gravity correction

	_plane->nav_acc_north = acc_world(0);
	_plane->nav_acc_east = acc_world(1);
	_plane->nav_acc_down = acc_world(2);
}

void Navigation::read_gnss()
{
	// Convert from lat/lon to meters
	gnss_n = _plane->gnss_lat;
	gnss_e = _plane->gnss_lon;
	gnss_d = _plane->gnss_asl;
	last_gnss_timestamp = _plane->gnss_timestamp;
}

bool Navigation::check_new_imu_data()
{
    return last_imu_timestamp != _plane->imu_timestamp;
}

bool Navigation::check_new_gnss_data()
{
	return last_gnss_timestamp != _plane->gnss_timestamp;
}

bool Navigation::check_new_baro_data()
{
	return last_baro_timestamp != _plane->baro_timestamp;
}


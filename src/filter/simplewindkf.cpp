/*
 *  Copyright (c) 2018 Julian Soeren Lorenz, Carnegie Mellon University, All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *	following conditions are met:
 *
 *   	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *   	   following disclaimer.
 *   	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   	   following disclaimer in the documentation and/or other materials provided with the distribution.
 *   	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *   	   products derived from this software without specific prior written permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *	OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	END OF LICENSE
 *
 *	Author: Julian Soeren Lorenz
 *	Email:  JulianLorenz@live.de
 *
*/

#include "simplewindkf.h"

SimpleWindKF::SimpleWindKF(){
	modelVarGroundspeed = 1;
	modelVarWindspeed = 0.00001;
	measurementVarGroundspeed = 1;
	measurementVarAirspeed = 2;
}

// Sets the model matrix A
void SimpleWindKF::setA(){
	A << MatrixXd::Identity(6,6);
}

// Sets the observation matrix H
void SimpleWindKF::setH(){
	H << 1, 0, 0, 0, 0, 0,
		 0, 1, 0, 0, 0, 0,
		 0, 0, 1, 0, 0, 0,
		 1, 0, 0,-1, 0, 0,
		 0, 1, 0, 0,-1, 0,
		 0, 0, 1, 0, 0,-1;
}

// Sets the measurement covariance matrix
void SimpleWindKF::setR(){
	R.diagonal() << measurementVarGroundspeed,
					measurementVarGroundspeed,
					measurementVarGroundspeed,
					measurementVarAirspeed,
					measurementVarAirspeed,
					measurementVarAirspeed;
	R = R*dt;
}

// Sets the model covariance matrix
void SimpleWindKF::setQ(){
	Q.diagonal() << modelVarGroundspeed,
					modelVarGroundspeed,
					modelVarGroundspeed,
					modelVarWindspeed,
					modelVarWindspeed,
					modelVarWindspeed;
	Q = Q*dt;
}

// Estimates the current state x with measurement z and updates the wind vector in WindFilter class
void SimpleWindKF::estimate(MeasurementType z){
	KalmanFilter::estimate(z);
	setWind(x.tail(3));
}

// Computes the state error covariance P and extracts the variances for the wind to the WindFilter class
void SimpleWindKF::computeErrorCovariance(){
	KalmanFilter::computeErrorCovariance();
	setWindSigmaWithVariance(P.diagonal().tail(3));
}

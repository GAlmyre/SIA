/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <warp.h>
#include <vector.h>

Point2f Warp::squareToUniformSquare(const Point2f &sample) {
    return sample;
}

float Warp::squareToUniformSquarePdf(const Point2f &sample) {
    return ((sample.array() >= 0).all() && (sample.array() <= 1).all()) ? 1.0f : 0.0f;
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
  double r = sqrt(sample.x());
  double phi = 2*M_PI*sample.y();

  double x = r*cos(phi);
  double y = r*sin(phi);

  return Point2f(x,y);
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
  double r = sqrt(p.x()*p.x()+p.y()*p.y());

  return ((r >= 0) && (r <= 1)) ? 1.0f/M_PI : 0.0f;
}

Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
  double phi = 2*M_PI*sample.x();
  double theta = acos(sample.y());

  return Vector3f(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
  double norme = v.norm();
  double theta = acos(v.z());

  return ((norme - 1.f < Epsilon) && (theta >= -M_PI/2 && theta <= M_PI/2)) ? 1./(2*M_PI) : 0.0f;
}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
    double phi = 2*M_PI*sample.x();
    double theta = acos(sqrt(1-sample.y()));

    return Vector3f(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {
  double norme = v.norm();
  double theta = acos(v.z());

  return ((norme - 1.f < Epsilon) && (theta >= -M_PI/2 && theta <= M_PI/2)) ? cos(theta)*(1./M_PI) : 0.0f;
}

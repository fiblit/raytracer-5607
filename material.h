#ifndef MATERIAL_H
#define MATERIAL_H

#include "rgb.h"

class material
{
	public:
        material();
        material(rgb Od, rgb Os, double ka, double kd, double ks, int n, double opcaity, double eta);
		rgb getOd();
		rgb getOs();
		double getka();
		double getkd();
		double getks();
		int getn();
		double getOpacity();
		double getEta();
		void setOd(rgb Od);
		void setOs(rgb Os);
		void setka(double ka);
		void setkd(double kd);
		void setks(double ks);
		void setn(int n);
		void setOpacity(double opacity);
		void setEta(double eta);
	private:
		rgb Od;
		rgb Os;
		double ka;
		double kd;
		double ks;
		int n;
		double opacity;
		double eta;
};

#endif

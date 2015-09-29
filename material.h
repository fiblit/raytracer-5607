#ifndef MATERIAL_H
#define MATERIAL_H

#include "rgb.h"

class material
{
	public:
		rgb getOd();
		rgb getOs();
		int getka();
		int getkd();
		int getks();
		int getn();
		void setOd(rgb Od);
		void setOs(rgb Os);
		void setka(int ka);
		void setkd(int ks);
		void setks(int ks);
		void setn(int n);
	private:
		rgb Od;
		rgb Os;
		int ka;
		int kd;
		int ks;
		int n;
}

#endif
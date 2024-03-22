#include <stdio.h>
#include <stdlib.h>

typedef
struct {
	int exp;
	double coef;
}
term_t;

typedef
struct {
	int degree;
	int n_terms;
	term_t* terms; // a list of attached terms 
}
poly_t;

poly_t* poly_zero()
{
	poly_t* p = (poly_t*)malloc(sizeof(poly_t));

	p->degree = 0;
	p->n_terms = 0;
	p->terms = NULL;
	return p;
}

int poly_degree(poly_t* p)
{
	return p->degree;
}

double poly_coef(poly_t* p, int exp)
{
	if (exp < 0)
		return 0.0;
	if (p->degree < exp)
		return 0.0;

	int i;
	for (i = 0; i < p->n_terms; i++) {
		if ((p->terms[i]).exp == exp) {
			return (p->terms[i]).coef;
		}
	}
	return 0.0;
}

poly_t* poly_attach(poly_t* p, double coef, int exp)
{
	if (exp < 0)
		return NULL;
	if (coef == 0.0)
		return NULL;
	if (poly_coef(p, exp) != 0.0)
		return NULL;

	if (p->degree < exp) {
		p->degree = exp;
	}
	p->terms = realloc(p->terms, sizeof(term_t) * (p->n_terms + 1));
	(p->terms[p->n_terms]).coef = coef;
	(p->terms[p->n_terms]).exp = exp;
	p->n_terms += 1;

	return p;
}

poly_t* poly_remove(poly_t* p, int exp)
{
	if (exp < 0)
		return NULL;
	if (p->degree < exp)
		return NULL;
	if (poly_coef(p, exp) == 0.0)
		return NULL;

	int i = 0;
	while (p->terms[i].exp != exp) {
		i++;
	}
	// assert p->terms[i].exp == exp

	for (; i < p->n_terms - 1; i++) {
		p->terms[i] = p->terms[i + 1];
	}
	p->terms = realloc(p->terms, (p->n_terms - 1) * sizeof(term_t));
	p->n_terms -= 1;

	int j;
	p->degree = 0;
	for (j = 0; j < p->n_terms; j++) {
		if (p->degree < p->terms[j].exp) {
			p->degree = p->terms[j].exp;
		}
	}

	return p;
}


void poly_delete(poly_t* p)
{
	if (p->terms != NULL) {
		free(p->terms);
	}
	free(p);
}

poly_t* poly_scale(poly_t* p, double s)
{
	for (int i = 0; i < p->n_terms; i++) {
		(p->terms[i].coef) = (p->terms[i].coef) * s;
	}
	return p;
}

 poly_t * poly_add (poly_t * p1, poly_t * p2)
 {
	 poly_t * add_p = poly_zero();
	 int degree1 = poly_degree(p1); // p1의 차수
	 int degree2 = poly_degree(p2); // p2의 차수
	 int degree = degree1 > degree2 ? degree1 : degree2; // added_p의 차수
	 printf("둘 중 최고 차수: %d\n", degree);
	 for (int i = degree; i >= 0; i--) {
		 double coef = poly_coef(p1, i) + poly_coef(p2, i);
		 printf("%d차: %f\n", i, coef);
		 poly_attach(add_p, coef, i);
	 }
	 return add_p;
 }

poly_t * poly_sub (poly_t * p1, poly_t * p2)
{
	poly_t* sub_p = poly_zero();
	int degree1 = poly_degree(p1);
	int degree2 = poly_degree(p2);
	int degree = degree1 > degree2 ? degree1 : degree2; // added_p의 차수
	printf("둘 중 최고 차수: %d\n", degree);
	for (int i = degree; i >= 0; i--) {
		double coef = poly_coef(p1, i) - poly_coef(p2, i);
		printf("%d차: %f\n", i, coef);
		poly_attach(sub_p, coef, i);
	}
	return sub_p;
}

poly_t * poly_mult (poly_t * p1, poly_t * p2)
{
	poly_t* mult_p = poly_zero();
	for (int i = 0; i < p1->n_terms; i++) {
		for (int j = 0; j < p2->n_terms; j++) {
			poly_t* new_p = poly_zero();
			double coef = (p1->terms[i]).coef * (p2->terms[j]).coef;
			int exp = (p1->terms[i]).exp + (p2->terms[j]).exp;
			poly_attach(new_p, coef, exp);
			mult_p = poly_add(mult_p, new_p);
		}
	}
	return mult_p;
}

	
double poly_eval (poly_t * p, double x)
{
	double ans = 0;
	for (int i = 0; i < p->n_terms; i++) {
		double value = 1;
		int coef = (p->terms[i].coef);
		int exp = (p->terms[i]).exp;
		while (exp > 0) {
			value *= x;
			exp--;
		}
		ans += value * coef;
	}
	return ans;
}



void poly_show(poly_t* p)
{
	int i;
	for (i = 0; i < p->n_terms; i++) {
		printf("%lf", p->terms[i].coef);
		if (p->terms[i].exp != 0) {
			printf("x^%d", p->terms[i].exp);
		}
		if (i != p->n_terms - 1) {
			printf("+");
		}
		printf(" ");
	}
	printf("\n");
}

int main()
{
	//1.0x^2 + 2.0x+ 1.0 
	poly_t* p = poly_zero();
	printf("%d\n", p == poly_attach(p, 1.0, 2));
	printf("%d\n", p == poly_attach(p, 2.0, 1));
	printf("%d\n", p == poly_attach(p, 1.0, 0));
	printf("%d\n", p == poly_attach(p, 2.0, 2));
	poly_show(p);

	printf("%d\n", p == poly_remove(p, 1));
	poly_show(p);

	poly_delete(p);
	
	
	printf("\nHomework Part\n");
	printf("====================================================================\n");
	printf("\npoly_scale\n");
	printf("--------------------------------------------------------------------\n");
	poly_t* p1 = poly_zero(); 
	printf("p1 ==  poly_attach(p1, 1.0, 2): %d\n", p1 == poly_attach(p1, 1.0, 2));
	printf("p1 ==  poly_attach(p1, 2.0, 1): %d\n", p1 == poly_attach(p1, 2.0, 1));
	printf("p1 ==  poly_attach(p1, 1.0, 0): %d\n", p1 == poly_attach(p1, 1.0, 0));
	printf("p1 before scale: ");
	poly_show(p1); // 1.0x^2 + 2.0x + 1.0
	printf("p1 == poly_scale(p1, 2.0): %d\n", p1 == poly_scale(p1, 2.0));
	printf("p1 after scale: ");
	poly_show(p1); // 2.0x^2 + 4.0x + 2.0

	printf("\npoly_add\n");
	printf("-------------------------------------------------------------------\n");
	poly_t* p2 = poly_zero(); // 1.0x^3 + 3.0x^2 + 3.0x
	printf("%d\n", p2 == poly_attach(p2, 1.0, 3));
	printf("%d\n", p2 == poly_attach(p2, 3.0, 2));
	printf("%d\n", p2 == poly_attach(p2, 3.0, 1));
	printf("p1: ");
	poly_show(p1);
	printf("p2: ");
	poly_show(p2);
	printf("p1 + p2: ");
	poly_show(poly_add(p1, p2)); // 1.0x^3 + 5.0x^2 + 7.0x

	printf("\npoly_sub\n");
	printf("-------------------------------------------------------------------\n");
	printf("p1: "); // 2.0x^2 + 4.0x + 2.0
	poly_show(p1);
	printf("p2: "); // 1.0x^3 + 3.0x^2 + 3.0x
	poly_show(p2);
	printf("p1 - p2: ");
	poly_show(poly_sub(p1, p2)); // -1.0x^3 - 1.0x^2 + 1.0x + 2.0

	printf("\npoly_mult\n");
	printf("-------------------------------------------------------------------\n");
	printf("p1: "); // 2.0x^2 + 4.0x + 2.0
	poly_show(p1);
	printf("p2: "); // 1.0x^3 + 3.0x^2 + 3.0x
	poly_show(p2);
	
	poly_show(poly_mult(p1, p2)); 

	printf("\npoly_eval\n");
	printf("-------------------------------------------------------------------\n");
	printf("p1: "); // 2.0x^2 + 4.0x + 2.0
	poly_show(p1);

	printf("p1(2.0) 계산값: %f\n", poly_eval(p1, 2.0));
	
	poly_delete(p1);
	poly_delete(p2);
	
	return EXIT_SUCCESS;
}

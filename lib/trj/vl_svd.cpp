#include "vl_svd.h"

long long unsigned __float64_min__ = 0x0350000000000000;
long double float64_min;

long long unsigned __float64_max__ = 0x1CC0000000000000;
long double float64_max;

vlf_t __fmax__(vlf_t a, vlf_t b)
{
    if (a > b) return a;
    else return b;
}

static vlf_t nav_system_xnrm2(int32_t n, const vlf_t x[9], int32_t ix0)
{
    vlf_t y;
    vlf_t scale;
    int32_t kend;
    vlf_t absxk;
    vlf_t t;
    int32_t k;
    y = 0.0;
    if (n >= 1) {
        if (n == 1) {
            y = fabs(x[ix0 - 1]);
        } else {
            scale = float64_max;
            kend = ix0 + n - 1;
            for (k = ix0; k <= kend; k++) {
                absxk = fabs(x[k - 1]);
                if (absxk > scale) {
                    t = scale / absxk;
                    y = y * t * t + 1.0;
                    scale = absxk;
                } else {
                    t = absxk / scale;
                    y += t * t;
                }
            }

            y = scale * sqrt(y);
        }
    }

    return y;
}

static vlf_t nav_system_xnrm2_h(int32_t n, const vlf_t x[3], int32_t ix0)
{
    vlf_t y;
    vlf_t scale;
    int32_t kend;
    vlf_t absxk;
    vlf_t t;
    int32_t k;
    y = 0.0;
    if (n >= 1) {
        if (n == 1) {
            y = fabs(x[ix0 - 1]);
        } else {
            scale = float64_max;
            kend = ix0 + n - 1;
            for (k = ix0; k <= kend; k++) {
                absxk = fabs(x[k - 1]);
                if (absxk > scale) {
                    t = scale / absxk;
                    y = y * t * t + 1.0;
                    scale = absxk;
                } else {
                    t = absxk / scale;
                    y += t * t;
                }
            }

            y = scale * sqrt(y);
        }
    }

    return y;
}

static void nav_system_xaxpy_jk(int32_t n, vlf_t a, const vlf_t x[3], int32_t
    ix0, vlf_t y[9], int32_t iy0)
{
    int32_t ix;
    int32_t iy;
    int32_t k;
    if (n >= 1 && a != 0.0) {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y[iy] += a * x[ix];
            ix++;
            iy++;
        }
    }
}

static void nav_system_xaxpy_j(int32_t n, vlf_t a, const vlf_t x[9], int32_t
    ix0, vlf_t y[3], int32_t iy0)
{
    int32_t ix;
    int32_t iy;
    int32_t k;
    if (n >= 1 && a != 0.0) {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y[iy] += a * x[ix];
            ix++;
            iy++;
        }
    }
}

static vlf_t nav_system_xdotc(int32_t n, const vlf_t x[9], int32_t ix0, const
    vlf_t y[9], int32_t iy0)
{
    vlf_t d;
    int32_t ix;
    int32_t iy;
    int32_t k;
    d = 0.0;
    if (n >= 1) {
        ix = ix0;
        iy = iy0;
        for (k = 0; k < n; k++) {
            d += x[ix - 1] * y[iy - 1];
            ix++;
            iy++;
        }
    }

    return d;
}

static void nav_system_xaxpy(int32_t n, vlf_t a, int32_t ix0, vlf_t y[9],
    int32_t iy0)
{
    int32_t ix;
    int32_t iy;
    int32_t k;
    if (n >= 1 && a != 0.0) {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y[iy] += a * y[ix];
            ix++;
            iy++;
        }
    }
}

static void nav_system_xscal(vlf_t a, vlf_t x[9], int32_t ix0)
{
    int32_t k;
    for (k = ix0; k <= ix0 + 2; k++) {
        x[k - 1] *= a;
    }
}

static void nav_system_xswap(vlf_t x[9], int32_t ix0, int32_t iy0)
{
    int32_t ix;
    int32_t iy;
    vlf_t temp;
    ix = ix0 - 1;
    iy = iy0 - 1;
    temp = x[ix];
    x[ix] = x[iy];
    x[iy] = temp;
    ix++;
    iy++;
    temp = x[ix];
    x[ix] = x[iy];
    x[iy] = temp;
    ix++;
    iy++;
    temp = x[ix];
    x[ix] = x[iy];
    x[iy] = temp;
}

static void nav_system_xrotg(vlf_t *a, vlf_t *b, vlf_t *c, vlf_t *s)
{
    vlf_t roe;
    vlf_t absa;
    vlf_t absb;
    vlf_t scale;
    vlf_t ads;
    vlf_t bds;
    roe = *b;
    absa = fabs(*a);
    absb = fabs(*b);
    if (absa > absb) {
        roe = *a;
    }

    scale = absa + absb;
    if (scale == 0.0) {
        *s = 0.0;
        *c = 1.0;
        absa = 0.0;
    } else {
        ads = absa / scale;
        bds = absb / scale;
        scale *= sqrt(ads * ads + bds * bds);
        if (roe < 0.0) {
            scale = -scale;
        }

        *c = *a / scale;
        *s = *b / scale;
        if (absa > absb) {
            absa = *s;
        } else if (*c != 0.0) {
            absa = 1.0 / *c;
        } else {
            absa = 1.0;
        }
    }

    *a = scale;
    *b = absa;
}

static void nav_system_xrot(vlf_t x[9], int32_t ix0, int32_t iy0, vlf_t c,
    vlf_t s)
{
    int32_t ix;
    int32_t iy;
    vlf_t temp;
    ix = ix0 - 1;
    iy = iy0 - 1;
    temp = c * x[ix] + s * x[iy];
    x[iy] = c * x[iy] - s * x[ix];
    x[ix] = temp;
    iy++;
    ix++;
    temp = c * x[ix] + s * x[iy];
    x[iy] = c * x[iy] - s * x[ix];
    x[ix] = temp;
    iy++;
    ix++;
    temp = c * x[ix] + s * x[iy];
    x[iy] = c * x[iy] - s * x[ix];
    x[ix] = temp;
}

void nav_system_svd(vlf_t A[9], vlf_t U[9], vlf_t s[3], vlf_t V[9])
{
    float64_min = *((long double*) &__float64_min__);
    float64_max = *((long double*) &__float64_max__);
    
    vlf_t b_A[9];
    vlf_t b_s[3];
    vlf_t e[3];
    vlf_t work[3];
    int32_t qq;
    uint8_t apply_transform;
    vlf_t nrm;
    int32_t qjj;
    int32_t m;
    vlf_t rt;
    int32_t kase;
    vlf_t ztest;
    vlf_t smm1;
    vlf_t emm1;
    vlf_t sqds;
    vlf_t shift;
    int32_t k_ii;
    int32_t d;
    int32_t exitg1;
    uint8_t exitg2;
    e[0] = 0.0;
    work[0] = 0.0;
    e[1] = 0.0;
    work[1] = 0.0;
    e[2] = 0.0;
    work[2] = 0.0;
    for (qq = 0; qq < 9; qq++) {
        b_A[qq] = A[qq];
        U[qq] = 0.0;
        V[qq] = 0.0;
    }

    qq = 0;
    apply_transform = 0x00;
    nrm = nav_system_xnrm2(3, b_A, 1);
    if (nrm > 0.0) {
        apply_transform = 0x01;
        if (b_A[0] < 0.0) {
            b_s[0] = -nrm;
        } else {
            b_s[0] = nrm;
        }

        if (fabs(b_s[0]) >= float64_min) {
            nrm = 1.0 / b_s[0];
            for (qjj = 0; qjj < 3; qjj++) {
                b_A[qjj] *= nrm;
            }
        } else {
            for (qjj = 0; qjj < 3; qjj++) {
                b_A[qjj] /= b_s[0];
            }
        }

        b_A[0]++;
        b_s[0] = -b_s[0];
    } else {
        b_s[0] = 0.0;
    }

    for (d = 1; d + 1 < 4; d++) {
        qjj = 3 * d;
        if (apply_transform) {
            nav_system_xaxpy(3, -(nav_system_xdotc(3, b_A, 1, b_A, qjj + 1) /
                                  b_A[0]), 1, b_A, qjj + 1);
        }

        e[d] = b_A[qjj];
    }

    while (qq + 1 < 4) {
        U[qq] = b_A[qq];
        qq++;
    }

    nrm = nav_system_xnrm2_h(2, e, 2);
    if (nrm == 0.0) {
        e[0] = 0.0;
    } else {
        if (e[1] < 0.0) {
            nrm = -nrm;
        }

        e[0] = nrm;
        if (fabs(nrm) >= float64_min) {
            nrm = 1.0 / nrm;
            for (qq = 1; qq < 3; qq++) {
                e[qq] *= nrm;
            }
        } else {
            for (qq = 1; qq < 3; qq++) {
                e[qq] /= nrm;
            }
        }

        e[1]++;
        e[0] = -e[0];
        for (qq = 2; qq < 4; qq++) {
            work[qq - 1] = 0.0;
        }

        for (qq = 1; qq + 1 < 4; qq++) {
            nav_system_xaxpy_j(2, e[qq], b_A, 3 * qq + 2, work, 2);
        }

        for (qq = 1; qq + 1 < 4; qq++) {
            nav_system_xaxpy_jk(2, -e[qq] / e[1], work, 2, b_A, 3 * qq + 2);
        }
    }

    for (qq = 1; qq + 1 < 4; qq++) {
        V[qq] = e[qq];
    }

    apply_transform = 0x00;
    nrm = nav_system_xnrm2(2, b_A, 5);
    if (nrm > 0.0) {
        apply_transform = 0x01;
        if (b_A[4] < 0.0) {
            b_s[1] = -nrm;
        } else {
            b_s[1] = nrm;
        }

        if (fabs(b_s[1]) >= float64_min) {
            nrm = 1.0 / b_s[1];
            for (qjj = 4; qjj < 6; qjj++) {
                b_A[qjj] *= nrm;
            }
        } else {
            for (qjj = 4; qjj < 6; qjj++) {
                b_A[qjj] /= b_s[1];
            }
        }

        b_A[4]++;
        b_s[1] = -b_s[1];
    } else {
        b_s[1] = 0.0;
    }

    for (d = 2; d + 1 < 4; d++) {
        qjj = 3 * d + 1;
        if (apply_transform) {
            nav_system_xaxpy(2, -(nav_system_xdotc(2, b_A, 5, b_A, qjj + 1) /
                                  b_A[4]), 5, b_A, qjj + 1);
        }

        e[d] = b_A[qjj];
    }

    for (qq = 1; qq + 1 < 4; qq++) {
        U[qq + 3] = b_A[qq + 3];
    }

    m = 1;
    b_s[2] = b_A[8];
    U[6] = 0.0;
    U[7] = 0.0;
    U[8] = 1.0;
    for (d = 1; d >= 0; d--) {
        qq = 3 * d + d;
        if (b_s[d] != 0.0) {
            for (kase = d + 1; kase + 1 < 4; kase++) {
                qjj = 3 * kase + d + 1;
                nav_system_xaxpy(3 - d, -(nav_system_xdotc(3 - d, U, qq + 1, U,
                                   qjj) / U[qq]), qq + 1, U, qjj);
            }

            for (qjj = d; qjj + 1 < 4; qjj++) {
                U[qjj + 3 * d] = -U[3 * d + qjj];
            }

            U[qq]++;
            if (0 <= d - 1) {
                U[3 * d] = 0.0;
            }
        } else {
            U[3 * d] = 0.0;
            U[1 + 3 * d] = 0.0;
            U[2 + 3 * d] = 0.0;
            U[qq] = 1.0;
        }
    }

    for (qq = 2; qq >= 0; qq--) {
        if (qq + 1 <= 1 && e[0] != 0.0) {
            for (d = 2; d < 4; d++) {
                qjj = (d - 1) * 3 + 2;
                nav_system_xaxpy(2, -(nav_system_xdotc(2, V, 2, V, qjj) / V[1]),
                                 2, V, qjj);
            }
        }

        V[3 * qq] = 0.0;
        V[1 + 3 * qq] = 0.0;
        V[2 + 3 * qq] = 0.0;
        V[qq + 3 * qq] = 1.0;
    }

    sqds = e[0];
    if (b_s[0] != 0.0) {
        rt = fabs(b_s[0]);
        nrm = b_s[0] / rt;
        b_s[0] = rt;
        sqds = e[0] / nrm;
        nav_system_xscal(nrm, U, 1);
    }

    if (sqds != 0.0) {
        rt = fabs(sqds);
        nrm = rt / sqds;
        sqds = rt;
        b_s[1] *= nrm;
        nav_system_xscal(nrm, V, 4);
    }

    e[0] = sqds;
    sqds = b_A[7];
    if (b_s[1] != 0.0) {
        rt = fabs(b_s[1]);
        nrm = b_s[1] / rt;
        b_s[1] = rt;
        sqds = b_A[7] / nrm;
        nav_system_xscal(nrm, U, 4);
    }

    if (sqds != 0.0) {
        rt = fabs(sqds);
        nrm = rt / sqds;
        sqds = rt;
        b_s[2] = b_A[8] * nrm;
        nav_system_xscal(nrm, V, 7);
    }

    e[1] = sqds;
    if (b_s[2] != 0.0) {
        rt = fabs(b_s[2]);
        nrm = b_s[2] / rt;
        b_s[2] = rt;
        nav_system_xscal(nrm, U, 7);
    }

    e[2] = 0.0;
    qq = 0;
    nrm = __fmax__(__fmax__(__fmax__(b_s[0], e[0]), __fmax__(b_s[1], sqds)), __fmax__(b_s[2], 0.0));
    while (m + 2 > 0 && qq < 75) {
        kase = m + 1;
        do {
            exitg1 = 0;
            d = kase;
            if (kase == 0) {
                exitg1 = 1;
            } else {
                rt = fabs(e[kase - 1]);
                if (rt <= (fabs(b_s[kase - 1]) + fabs(b_s[kase])) *
                        2.2204460492503131E-16) {
                    e[kase - 1] = 0.0;
                    exitg1 = 1;
                } else if (((rt <= float64_min) || (qq > 20)) && (rt <=
                           2.2204460492503131E-16 * nrm)) {
                    e[kase - 1] = 0.0;
                    exitg1 = 1;
                } else {
                    kase--;
                }
            }
        } while (exitg1 == 0);

        if (m + 1 == kase) {
            kase = 4;
        } else {
            qjj = m + 2;
            k_ii = m + 2;
            exitg2 = 0x00;
            while (!exitg2 && k_ii >= kase) {
                qjj = k_ii;
                if (k_ii == kase) {
                    exitg2 = 0x01;
                } else {
                    rt = 0.0;
                    if (k_ii < m + 2) {
                        rt = fabs(e[k_ii - 1]);
                    }

                    if (k_ii > kase + 1) {
                        rt += fabs(e[k_ii - 2]);
                    }

                    ztest = fabs(b_s[k_ii - 1]);
                    if (ztest <= 2.2204460492503131E-16 * rt || ztest <=
                            float64_min) {
                        b_s[k_ii - 1] = 0.0;
                        exitg2 = 0x01;
                    } else {
                        k_ii--;
                    }
                }
            }

            if (qjj == kase) {
                kase = 3;
            } else if (m + 2 == qjj) {
                kase = 1;
            } else {
                kase = 2;
                d = qjj;
            }
        }

        switch (kase) {
          case 1:
            rt = e[m];
            e[m] = 0.0;
            for (qjj = m; qjj + 1 >= d + 1; qjj--) {
                nav_system_xrotg(&b_s[qjj], &rt, &ztest, &sqds);
                if (qjj + 1 > d + 1) {
                    rt = -sqds * e[0];
                    e[0] *= ztest;
                }

                nav_system_xrot(V, 1 + 3 * qjj, 1 + 3 * (m + 1), ztest, sqds);
            }
            break;

          case 2:
            rt = e[d - 1];
            e[d - 1] = 0.0;
            for (qjj = d; qjj < m + 2; qjj++) {
                nav_system_xrotg(&b_s[qjj], &rt, &ztest, &sqds);
                rt = -sqds * e[qjj];
                e[qjj] *= ztest;
                nav_system_xrot(U, 1 + 3 * qjj, 1 + 3 * (d - 1), ztest, sqds);
            }
            break;

          case 3:
            rt = b_s[m + 1];
            ztest = __fmax__(__fmax__(__fmax__(__fmax__(fabs(rt), fabs(b_s[m])), fabs(e[m])),
                              fabs(b_s[d])), fabs(e[d]));
            rt /= ztest;
            smm1 = b_s[m] / ztest;
            emm1 = e[m] / ztest;
            sqds = b_s[d] / ztest;
            smm1 = ((smm1 + rt) * (smm1 - rt) + emm1 * emm1) / 2.0;
            emm1 *= rt;
            emm1 *= emm1;
            if (smm1 != 0.0 || emm1 != 0.0) {
                shift = sqrt(smm1 * smm1 + emm1);
                if (smm1 < 0.0) {
                    shift = -shift;
                }

                shift = emm1 / (smm1 + shift);
            } else {
                shift = 0.0;
            }

            rt = (sqds + rt) * (sqds - rt) + shift;
            ztest = e[d] / ztest * sqds;
            for (qjj = d + 1; qjj <= m + 1; qjj++) {
                nav_system_xrotg(&rt, &ztest, &sqds, &smm1);
                if (qjj > d + 1) {
                    e[0] = rt;
                }

                ztest = e[qjj - 1];
                emm1 = b_s[qjj - 1];
                rt = emm1 * sqds + ztest * smm1;
                e[qjj - 1] = ztest * sqds - emm1 * smm1;
                ztest = smm1 * b_s[qjj];
                b_s[qjj] *= sqds;
                nav_system_xrot(V, 1 + 3 * (qjj - 1), 1 + 3 * qjj, sqds, smm1);
                nav_system_xrotg(&rt, &ztest, &sqds, &smm1);
                b_s[qjj - 1] = rt;
                ztest = e[qjj - 1];
                rt = ztest * sqds + smm1 * b_s[qjj];
                b_s[qjj] = ztest * -smm1 + sqds * b_s[qjj];
                ztest = smm1 * e[qjj];
                e[qjj] *= sqds;
                nav_system_xrot(U, 1 + 3 * (qjj - 1), 1 + 3 * qjj, sqds, smm1);
            }

            e[m] = rt;
            qq++;
            break;

          default:
            if (b_s[d] < 0.0) {
                b_s[d] = -b_s[d];
                nav_system_xscal(-1.0, V, 1 + 3 * d);
            }

            qq = d + 1;
            while (d + 1 < 3 && b_s[d] < b_s[qq]) {
                rt = b_s[d];
                b_s[d] = b_s[qq];
                b_s[qq] = rt;
                nav_system_xswap(V, 1 + 3 * d, 1 + 3 * (d + 1));
                nav_system_xswap(U, 1 + 3 * d, 1 + 3 * (d + 1));
                d = qq;
                qq++;
            }

            qq = 0;
            m--;
            break;
        }
    }

    s[0] = b_s[0];
    s[1] = b_s[1];
    s[2] = b_s[2];
    
    return;
}

//------------------------------------------------------------------------------

void vl_svd(vlf_t *mat, vlf_t *u, vlf_t *a, vlf_t *v)
{
    nav_system_svd(mat, u, a, v);
    
    return;
}

//------------------------------------------------------------------------------

void vl_rnorm(vlf_t *mat)
{
    vlf_t u[9];
    vlf_t a[9];
    vlf_t v[9];
    vlf_t v_t[9];
    
    // smh matrices close to det(A) == 1 cant be SVDed
    if (fabsl(1.0 - vl_det(mat)) > 1E-9)
    {
        vl_svd(mat, u, a, v);
        vl_tnp(v_t, v);
        vl_mmul_m(mat, u, v_t);
    }
    
//    u, a, v = scipy.linalg.svd(rot);
//    return numpy.mat(u.dot(v));
    
    return;
}

//------------------------------------------------------------------------------

namespace {

// 2-region NASA coefficients
const double h2o_nasa_coeffs[] = {
    1000.0, 3.03399249E+00, 2.17691804E-03, -1.64072518E-07,
    -9.70419870E-11, 1.68200992E-14, -3.00042971E+04, 4.96677010E+00,
    4.19864056E+00, -2.03643410E-03, 6.52040211E-06, -5.48797062E-09,
    1.77197817E-12, -3.02937267E+04, -8.49032208E-01};

const double h2oliq_nasa_coeffs[] = {
    600.0, 7.255750050E+01, -6.624454020E-01, 2.561987460E-03,
    -4.365919230E-06, 2.781789810E-09, -4.188671E+04, -2.8827879E+02,
    7.255750050E+01, -6.624454020E-01, 2.561987460E-03, -4.365919230E-06,
    2.781789810E-09, -4.188671E+04, -2.8827879E+02};

const double h2o_comp[] = {2.0, 1.0, 0.0};

const double h2_nasa_coeffs[] = {
    1000.0, 3.33727920E+00, -4.94024731E-05, 4.99456778E-07,
    -1.79566394E-10, 2.00255376E-14, -9.50158922E+02, -3.20502331E+00,
    2.34433112E+00, 7.98052075E-03, -1.94781510E-05, 2.01572094E-08,
    -7.37611761E-12, -9.17935173E+02, 6.83010238E-01};
const double h2_comp[] = {2.0, 0.0, 0.0};

const double o2_nasa_coeffs[] = {
    1000.0, 3.28253784E+00, 1.48308754E-03, -7.57966669E-07,
    2.09470555E-10, -2.16717794E-14, -1.08845772E+03, 5.45323129E+00,
    3.78245636E+00, -2.99673416E-03, 9.84730201E-06, -9.68129509E-09,
    3.24372837E-12, -1.06394356E+03, 3.65767573E+00};
const double o2_comp[] = {0.0, 2.0, 0.0};

const double oh_nasa_coeffs[] = {
    1000.0, 3.09288767E+00, 5.48429716E-04, 1.26505228E-07,
    -8.79461556E-11, 1.17412376E-14, 3.85865700E+03, 4.47669610E+00,
    3.99201543E+00, -2.40131752E-03, 4.61793841E-06, -3.88113333E-09,
    1.36411470E-12, 3.61508056E+03, -1.03925458E-01};
const double oh_comp[] = {1.0, 1.0, 0.0};

// 2-region Shomate coefficients (from NIST Chemistry WebBook)
const double co2_shomate_coeffs[] = {
    1200.0, 24.99735, 55.18696, -33.69137, 7.948387, -0.136638, -403.6075, 228.2431,
    58.16639, 2.720074, -0.492289, 0.038844, -6.447293, -425.9186, 263.6125};
const double co2_comp[] = {0.0, 2.0, 1.0};

const double co_shomate_coeffs[] = {
    1300.0, 25.56759, 6.096130, 4.054656, -2.671301, 0.131021, -118.0089, 227.3665,
    35.15070, 1.300095, -0.205921, 0.013550, -3.282780, -127.8375, 231.7120};
const double co_comp[] = {0.0, 1.0, 1.0};

const double li7si3_shomate_coeffs[] = {
    700.0, 295.73961, -6.753295, -44.538551, 29.738846, -1.022387, -348.88919,
    554.35647, 250.51429, 51.125155, -28.341244, 6.242135, 1.346861, -328.46578,
    498.84106};

// single-region Shomate coefficients
const double kcl_shomate_coeffs[] = {
    73.59698, 0.0, 0.0, 0.0, 0.0, -443.7341, 175.7209};

const double licl_shomate_coeffs[] = {
    73.18025, -9.047232, -0.316390, 0.079587, 0.013594, -417.1314, 157.6711};
}
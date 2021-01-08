#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "bntseq.h"
#include "bwaseqio.h"
#include "splicescore.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
/*
double global_score_matrix[4][60] = {
//in the order of ACGTN
    { 9903,10250,9592,9801,10503,9740,10161,10326,9553,10507,10826,10951,12470,23711,3708,  20,
      5,22925,25631,2849,6248,11163,8213,7834,8113,8210,7789,7694,8026,7666,4326,
      3886,3561,3287,2853,2961,3309,3815,3858,3077,2923,9104,1962,36255,   0,9404,
      8460,9430,9009,9523,9452,9747,10462,9385,9937,10064,9608,9634,10245,9233},
      
    { 9211,9269,9685,9515,9189,9351,9028,8819,8857,8322,8581,9606,13087,3812, 995,  20,
       184, 901,2655,2040,5641,7281,8821,9832,9067,9064,8809,9150,8785,8972,10269,
       10366,10314,10166,9690,10122,10907,12084,12375,12657,10881,9854,23399,   1,  30,4968,
       6978,8571,9427,9059,8867,8864,8555,8456,8611,8229,8536,8614,8569,9031},
       
    { 9289,8259,9010,8912,8005,8962,9058,7812,8989,8812,7185,7934,6384,3680,28929,36222,
          1,11459,3889,28798,6557,10097,8441,8663,8857,9192,9008,8890,8872,9083,4735,
	  4683,4397,3998,3801,4114,4242,3880,3300,2204,2189,7260,  64,   1,36226,17859,
	  7167,8225,8034,7479,9110,9526,7900,9464,9584,8775,9684,9651,8303,9480},

    { 7859,8484,7975,8034,8565,8209,8015,9305,8863,8621,9670,7771,4321,5059,2630,   0,
      36072, 977,4087,2575,17816,7721,10787,9933,10225,9796,10656,10528,10579,10541,16932,
    17327,17990,18811,19918,19065,17804,16483,16729,18324,20269,10044,10837,   5,   6,4031,
      13657,10036,9792,10201,8833,8125,9345,8957,8130,9194,8434,8363,9145,8518},
   
};
*/
double global_score_matrix[4][60] = {
    {0.264642960393177,0.272523024821377,0.252137281625573,0.262375583364308,0.273646388303804,0.258530541444678,0.270412588279024,0.275252963284186,0.258447941188618,0.274943212323958,0.288770495188535,0.292016685251724,0.328699458968323,0.633242473051666,0.0995993887581051,0.00614132903812002,0.00600503861561971,0.59821996448189,0.679581216701772,0.0841283607979185,0.172973196216908,0.295989757568248,0.223152851773841,0.215115846859125,0.220348573080577,0.218457027216784,0.211126254491389,0.209618799818279,0.218580927600876,0.212080287448891,0.122231858918763,0.113653822326849,0.103828521868418,0.0972081113451452,0.0866228885309544,0.087932102589518,0.095729566761657,0.110349812084417,0.1145376450667,0.0931028786189237,0.092817907735514,0.251327799116177,0.0639036881014331,0.983963160285797,0.00512947590137529,0.257733448973692,0.249522983521249,0.258109280138768,0.247614917606245,0.265307892454467,0.264473629868253,0.270065667203568,0.288299673728989,0.260678148102259,0.270334118035766,0.281468632552761,0.262966175195143,0.271560731838269,0.2786684838723,0.256374674761492},
    {0.2573369677446,0.260157766489076,0.270606698880767,0.266109114938256,0.262565563953248,0.261694131251807,0.254536819064139,0.249667533969355,0.252397472432165,0.243241234047826,0.244794118861768,0.268516912402428,0.361206789741048,0.111295585016314,0.0307603353570396,0.00597199851319539,0.0159996695989758,0.0365547433197043,0.081580142898443,0.0645810102011316,0.162639904183703,0.198393425019618,0.250993268079131,0.268463222235989,0.247697517862305,0.251600379961178,0.245851402139347,0.253909057118077,0.24576054185768,0.24842440011564,0.282439185561475,0.280799570478668,0.279316895882377,0.275703134679718,0.261628051046958,0.278201792425557,0.295403295750217,0.322905051005658,0.333093792590757,0.342084830462974,0.296192128195597,0.265452442902573,0.639387932102589,0.00529467641349688,0.00626522942221121,0.147978358732912,0.191293933011192,0.235811341015157,0.265113781852724,0.252277702060876,0.241688349233883,0.244137446826085,0.234353446495684,0.234192375996366,0.238834510386982,0.231148556560525,0.237963077685541,0.241184487671912,0.238132408210465,0.246437863957378},
    {0.262941395118325,0.239730723165242,0.262276463057036,0.253859496964441,0.231830008673027,0.259984305951348,0.258076240036344,0.226865733283773,0.255259571304671,0.246384173790939,0.209499029446991,0.230735555280221,0.186676578697394,0.116102919919052,0.797022260769008,0.983244538058068,0.0057861479370586,0.328947259736505,0.122863750877628,0.774315450377896,0.187585181514063,0.291459133523314,0.239833973485318,0.24974187419981,0.258881592532937,0.264296039317722,0.26080204848635,0.256473795068765,0.253508445876182,0.260269276834758,0.136839714203114,0.130471234460827,0.12461900631892,0.115499938049808,0.108697806963202,0.114372444554578,0.121579316895882,0.110643042993433,0.0985792755957543,0.0696320158592492,0.0666253665386363,0.205158385990997,0.0102093916491141,0.00544335687440631,0.983702969479205,0.478672613885103,0.194965514393095,0.232891421963408,0.223421302606038,0.208739107091232,0.254169247924669,0.264097798703176,0.22644034196506,0.263845867922191,0.267938710610003,0.240763226366002,0.268867963490687,0.263234626027341,0.233725684549622,0.266959897575683},
    {0.215078676743898,0.227588485524305,0.214979556436625,0.217655804732995,0.231958039069921,0.219791021352166,0.216974352620493,0.248213769462685,0.233895015074547,0.235431379837277,0.256936356502705,0.208730847065626,0.123417172593235,0.139359022012968,0.0726180151158469,0.00464213439061661,0.972209143848346,0.0362780324619006,0.115974889522158,0.0769751786230537,0.476801718085326,0.21415768388882,0.286019906661711,0.266679056705076,0.273072316524181,0.265646553504316,0.282220294882914,0.279998347994879,0.282150084665262,0.27922603560071,0.458489241316648,0.475075372733655,0.492235575930285,0.511588815925329,0.543051253458886,0.519493660430347,0.487287820592244,0.456102093916491,0.453789286746789,0.495180275058853,0.544364597530252,0.278061371990253,0.286498988146863,0.00529880642629992,0.00490232519720811,0.115615578408293,0.364217569074464,0.273187956882666,0.263849997934994,0.273675298393425,0.239668772973196,0.221699087267171,0.250906537810267,0.241283607979185,0.222892660967249,0.246619584520712,0.230202783628629,0.224020154462479,0.249473423367612,0.230227563705447},
};

double global_score_background[4] = { 0.272,0.211,0.217,0.300 };

double global_logit_a = 1.127e+00 ;
double global_logit_b = -4.810e-05 ;
double global_logit_c = 2.771e-01 ;
double global_foreground_num = 1 ;


double get_splice_score(const ubyte_t *pacseq, int64_t ue_end_t, int64_t de_start_t)
{
    //get the sequence first
    int64_t flanking_size = 15;
    int64_t fragment_size = flanking_size*4;
    int64_t k;
    int i = 0;
    ubyte_t *ref_seq = (ubyte_t *) malloc(fragment_size * sizeof(ubyte_t));
    
    for(k = ue_end_t - flanking_size +1; k<ue_end_t + flanking_size +1; k++, i++ ){
	ref_seq[i] = get_pacseq_base (pacseq,k);
    }
    for(k = de_start_t - flanking_size ; k<de_start_t + flanking_size ; k++, i++ ){
        ref_seq[i] = get_pacseq_base (pacseq,k);
    }
    
    //check if it's the correct strand, if not, reverse the seq
    if(ref_seq[flanking_size] == BASE_G && ref_seq[flanking_size+1] == BASE_T );
    else{
	seq_reverse(fragment_size, ref_seq, 1);
    }
    //compare to the motif
    double seqs_score = get_seqs_score(fragment_size, ref_seq, global_score_matrix, global_score_background);
    free(ref_seq);
    int intron_size = de_start_t - ue_end_t;
    
    //use the regressed model
    double splice_score = -(global_logit_a + global_logit_b* (double)intron_size + global_logit_c*seqs_score);
    splice_score = 1.0/(1.0 + exp(splice_score));
    return (splice_score);
		    
}



double get_seqs_score (int64_t fragment_size, ubyte_t *ref_seq, double (&global_score_matrix)[4][60], double *global_score_background)
{
    double seqs_score = 0.0;
    for(int i=0; i<fragment_size; i++){
	if(ref_seq[i] == BASE_N) continue;
	seqs_score += log(global_score_matrix[int(ref_seq[i])][i] / (double)global_foreground_num / global_score_background[ int(ref_seq[i]) ]);
    }
    return seqs_score;
}

//void load_model_cfg(double *global_logit_a, double *global_logit_b, double *global_logit_c, double *global_foreground_num, double global_score_matrix[4][60], double *global_score_background, const char *cfg_file_name)
void load_model_cfg( const char *cfg_file_name)
{
    
    ifstream cfg_file(cfg_file_name);
    if(!cfg_file)
    {
	cerr << "Cannot open configuration file" << cfg_file_name <<endl;
	exit(1);
    }
    int line_buffer_size = 1024*1024;
    char cfg_file_line[line_buffer_size];
    char tag[24];
    int matrix_i = 0;
    while( cfg_file.getline(cfg_file_line, line_buffer_size) )
     {
	 if(strncmp(cfg_file_line, "#", 1) == 0 ){
	     sscanf(cfg_file_line, "# %s", tag);
	     continue;
	 }
	 if(strcmp(tag, "LOGIT_A") == 0){
	     sscanf(cfg_file_line, "%lf", &global_logit_a);
	 }
	 if(strcmp(tag, "LOGIT_B") == 0) {
	     sscanf(cfg_file_line, "%lf", &global_logit_b);
	 }
	 if(strcmp(tag, "LOGIT_C") == 0) {
	     sscanf(cfg_file_line, "%lf", &global_logit_c);
	     //cerr << "global_logit_c"<<global_logit_c<<endl;
	 }

	 if(strcmp(tag, "FOREGROUND_NUM") == 0){
	     sscanf(cfg_file_line, "%lf",&global_foreground_num);
	 }

	 if(strcmp(tag, "BACKGROUND") == 0){
	     istringstream ss(cfg_file_line);
	     string s;
	     for (int j=0; j<4; j++){
		 if(getline(ss, s, ',')){
		     sscanf(s.c_str(), "%lf", &(global_score_background[j]) );
		     //global_score_background++;
		     //cerr << global_score_background[j]<<"\t";
		 }
		 else{
		     cerr << "Read config file error:\t"<< tag << endl;
		     exit(1);
		 }
	     }
	 }

	 if(strcmp(tag, "MATRIX") == 0){
	     istringstream ss(cfg_file_line);
	     string s;
	     
	     for (int j=0; j<60; j++){
		 if(getline(ss, s, ',')){
		     sscanf(s.c_str(), "%lf",& (global_score_matrix[matrix_i][j]) );
			//cerr<<global_score_matrix[matrix_i][j]<<endl;
		 }
		 else{
		     cerr << "Read config file error:\t"<< tag << endl;
		     exit(1);
		     
		 }
	     }
	     matrix_i++;
	 }
     }
     if(matrix_i != 4)
     {
	 cerr << "Read config file error: MATRIX must be 4 by 60\n";
     }
    
    cfg_file.close();
    //exit(1);
}

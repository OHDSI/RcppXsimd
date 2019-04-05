#include "xsimd/xsimd.hpp"
#include "FeatureDetector/cpu_x86.h"

#include <Rcpp.h>
using namespace Rcpp;

//' Poll OS and CPU for SIMD feature support
//' 
//' Execute CPUID to poll operating system and central processing unit for 
//' single instruction, multiple data feature support.
//' 
//' @return List of operating sytem (OS) and hardware (HW) feature support; 
//' see CPUID Wiki page for flag definitions
//' 
//' @references \url{https://en.wikipedia.org/wiki/CPUID}
//'
//' @export
// [[Rcpp::export]] 
List getSimdFeatures() {
  FeatureDetector::cpu_x86 features;
  features.detect_host();
  
  List osList = List::create(
    Named("OS_x64") = features.OS_x64,
    Named("OS_AVX") = features.OS_AVX,
    Named("OS_AVX512") = features.OS_AVX512
  );
  
  List miscList = List::create(
    Named("HW_MMX") = features.HW_MMX,
    Named("HW_x64") = features.HW_x64,
    Named("HW_ABM") = features.HW_ABM,
    Named("HW_RDRAND") = features.HW_RDRAND,
    Named("HW_BMI1") = features.HW_BMI1,
    Named("HW_BMI2") = features.HW_BMI2,
    Named("HW_ADX") = features.HW_ADX,
    Named("HW_PREFETCHWT1") = features.HW_PREFETCHWT1,
    Named("HW_MPX") = features.HW_MPX
  );
  
  List sseList = List::create(
    Named("HW_SSE") = features.HW_SSE,
    Named("HW_SSE2") = features.HW_SSE2,
    Named("HW_SSE3") = features.HW_SSE3,
    Named("HW_SSSE3") = features.HW_SSSE3,
    Named("HW_SSE4.1") = features.HW_SSE41,
    Named("HW_SSE4.2") = features.HW_SSE42,
    Named("HW_SSE4a") = features.HW_SSE4a,
    Named("HW_AES") = features.HW_AES,
    Named("HW_SHA") = features.HW_SHA
  );
  
  List avxList = List::create(
    Named("HW_AVX") = features.HW_AVX,
    Named("HW_XOP") = features.HW_XOP,
    Named("HW_FMA3") = features.HW_FMA3,
    Named("HW_FMA4") = features.HW_FMA4,
    Named("HW_AVX2") = features.HW_AVX2
  );
  
  List avx512List = List::create(
    Named("HW_AVX512_F") = features.HW_AVX512_F,
    Named("HW_AVX512_PF") = features.HW_AVX512_PF,
    Named("HW_AVX512_ER") = features.HW_AVX512_ER,
    Named("HW_AVX512_CD") = features.HW_AVX512_CD,
    Named("HW_AVX512_VL") = features.HW_AVX512_VL,
    Named("HW_AVX512_BW") = features.HW_AVX512_BW,
    Named("HW_AVX512_DQ") = features.HW_AVX512_DQ,
    Named("HW_AVX512_IFMA") = features.HW_AVX512_IFMA,
    Named("HW_AVX512_VBMI") = features.HW_AVX512_VBMI
  );
  
  List mergedList = List::create(
    Named("VendorString") = FeatureDetector::cpu_x86::get_vendor_string(),
    osList, miscList, sseList, avxList, avx512List);
  
  Function unlist("unlist");
  return unlist(mergedList, _["recursive"] = false);
}

// -*- C++ -*-
// $Id: inline_my_meas.h,v 1.1 2007/11/21 16:37:48 bjoo Exp $
/*! \file
 * \brief Inline spectrum calculations
 *
 * Spectrum calculations
 */

#ifndef __inline_mymeas_h__
#define __inline_mymeas_h__

#include "chromabase.h"
#include "meas/inline/abs_inline_measurement.h"

namespace Chroma 
{ 
  // A namespace for this particular   measurement
  namespace InlineMyMeasEnv 
  {
    extern const std::string name;
    extern const bool registered;
  }

  //! Parameter structure
  struct InlineMyMeasParams 
  {
    // Default constructor -- forward declaration
    InlineMyMeasParams();

    // Construct from XML -- forward declaration
    InlineMyMeasParams(XMLReader& xml_in, const std::string& path);

    // Write myself out
    void write(XMLWriter& xml_out, const std::string& path);

    // How often should I measure me in an HMC run
    unsigned long frequency;

    // I need the ID of the named object for the prop
    std::string prop_id;

    // I need the ID of the named object for the gauge field
    std::string gauge_id;

    std::string xml_file; // Support output to own private XML File
  };


  class InlineMyMeas : public AbsInlineMeasurement 
  {
  public:
    // Constructor -- default -- do nothing
    ~InlineMyMeas() {}

    // Constructor -- from param struct -- copy param struct inside me
    InlineMyMeas(const InlineMyMeasParams& p) : params(p) {}

    // Constructor -- copy constructor -- copy param struct from argument
    InlineMyMeas(const InlineMyMeas& p) : params(p.params) {}

    // Boiler plate
    unsigned long getFrequency(void) const {return params.frequency;}

    //! Do the measurement
    void operator()(const unsigned long update_no,
		    XMLWriter& xml_out); 


  private:
    void func(const unsigned long update_no,
	      XMLWriter& xml_out);

    InlineMyMeasParams params;
  };

};

#endif

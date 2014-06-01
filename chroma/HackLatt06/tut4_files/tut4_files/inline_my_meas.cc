// $Id: inline_my_meas.cc,v 1.1 2007/11/21 16:37:48 bjoo Exp $
/*! \file
 * \brief Inline construction of spectrum
 *
 * Spectrum calculations
 */

#include "inline_my_meas.h"
/* 
#include "meas/inline/abs_inline_measurement_factory.h"
#include "util/info/proginfo.h"
#include "meas/glue/mesplq.h"
#include "io/param_io.h"
#include "io/qprop_io.h"
*/
#include "chroma.h"

using namespace QDP;

namespace Chroma 
{ 
  // Fill in the blanks
  namespace InlineMyMeasEnv 
  { 

    // Function to register with a factory
    // This is boilerplate stuff
    AbsInlineMeasurement* createMeasurement(XMLReader& xml_in, 
					    const std::string& path) 
    {
      // Create an InlineMyMeasParams from the XML
      // Use it to create an InlineMyMeas
      return new InlineMyMeas(InlineMyMeasParams(xml_in, path));
    }

    // The name of my measurement for the XML file
    // Change this for each measurement
    const std::string name = "MY_MEAS";

    // Register the measurement in the measurement factory
    const bool registered = TheInlineMeasurementFactory::Instance().registerObject(name, createMeasurement);

  };



  // Param stuff
  InlineMyMeasParams::InlineMyMeasParams() { frequency = 0; }

  InlineMyMeasParams::InlineMyMeasParams(XMLReader& xml_in, const std::string& path) 
  {
    try 
    {
      XMLReader paramtop(xml_in, path);

      if (paramtop.count("Frequency") == 1)
	read(paramtop, "Frequency", frequency);
      else
	frequency = 1;

      // Read in the output propagator/source configuration info
      read(paramtop, "NamedObject/prop_id", prop_id);

      // Get either the gauge_id in the XML or the ID of the default
      // field of no explicit ID exists.
      gauge_id =  InlineDefaultGaugeField::readGaugeId(paramtop, "NamedObject/gauge_id");

      if( paramtop.count("xml_file") != 0 ) { 
	read(paramtop, "xml_file", xml_file);
      }
      else { 
	xml_file == "";
      }
	
    }
    catch(const std::string& e) 
    {
      QDPIO::cerr << "Caught Exception reading XML: " << e << endl;
      QDP_abort(1);
    }
  }


  void
  InlineMyMeasParams::write(XMLWriter& xml_out, const std::string& path) 
  {
    push(xml_out, path);
    
    // Write out the output propagator/source configuration info
    QDP::write(xml_out, "prop_id", prop_id);

    QDP::write(xml_out, "gauge_id", gauge_id);

    if( xml_file != "" ){ 
      QDP::write(xml_out, "xml_file", xml_file);
    }


    pop(xml_out);
  }


  void 
  InlineMyMeas::operator()(long unsigned int update_no,
			   XMLWriter& xml_out) 
  {

    // This bit merely supports providing an external xml_file 
    // for this measurement
    if ( params.xml_file == "" ) { 
      
      func( update_no, xml_out );
    }
    else { 

      // Hash an XML file name from the user specified string
      // and the update number
      std::string xml_file = makeXMLFileName(params.xml_file, update_no);

      // IN the global output, make a note that the output went
      // to this separate XML file
      push(xml_out, "my_meas");
      write(xml_out, "update_no", update_no);
      write(xml_out, "xml_file", xml_file);
      pop(xml_out);

      XMLFileWriter xml(xml_file);
      func(update_no, xml);
    }
  }



  void 
  InlineMyMeas::func(unsigned long int update_no, XMLWriter& xml_out)
  {
    START_CODE();

    StopWatch measure_time;
    measure_time.reset();
    measure_time.start();


    // Test that the gauge configuration  and the propagator we need
    // exist in the map.
    XMLBufferWriter gauge_xml;
    XMLBufferWriter prop_xml;
    try
    {
      // Try and get at the gauge field if it doesn't exist 
      // an exception will be thrown.
      TheNamedObjMap::Instance().getData< multi1d<LatticeColorMatrix> >(params.gauge_id);
      TheNamedObjMap::Instance().get(params.gauge_id).getRecordXML(gauge_xml);

      // Do the same with the propagator 
      TheNamedObjMap::Instance().getData<LatticePropagator>(params.prop_id);

      // Get its record XML
      TheNamedObjMap::Instance().get(params.gauge_id).getRecordXML(prop_xml);

    }
    catch( std::bad_cast ) 
    {

      // If the object exists and is of the wrong type we will 
      // land in this catch.
      QDPIO::cerr << InlineMyMeasEnv::name << ": caught dynamic cast error" 
		  << endl;
      QDP_abort(1);
    }
    catch (const string& e) 
    {
      // If the object is not in the map we will land in this 
      // catch
      QDPIO::cerr << InlineMyMeasEnv::name << ": map call failed: " << e 
		  << endl;
      QDP_abort(1);
    }

    // If we got here, then the gauge field and prop are both in 
    // the map. Their XML will have been captured.
    // Let us bind the references to a local name so 
    // we don't have to type the long lookup string every time.
    //
    // Note const here means we can't change the field
    const multi1d<LatticeColorMatrix>& u = 
      TheNamedObjMap::Instance().getData< multi1d<LatticeColorMatrix> >(params.gauge_id);


    const LatticePropagator& prop = 
      TheNamedObjMap::Instance().getData<LatticePropagator>(params.prop_id);



    QDPIO::cout << InlineMyMeasEnv::name <<": Beginning " << endl;

    // Boilerplate stuff to the output XML
    push(xml_out, "my_measurement");
    write(xml_out, "update_no", update_no);

    // Write info about the program
    proginfo(xml_out);

    // Write out the input
    params.write(xml_out, "Input");

    //----------------------------------------------------------------------
    // Compute our thing here....
    //
    //
    //
    //---------------------------------------------------------------------

    // End of  your code.
    pop(xml_out);


    measure_time.stop();
    QDPIO::cout << InlineMyMeasEnv::name << ": total time = "
		<< measure_time.getTimeInSeconds() 
		<< " secs" << endl;

    QDPIO::cout << InlineMyMeasEnv::name << ": ran successfully" << endl;
    END_CODE();
  } 

};

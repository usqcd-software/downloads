// $Id: tut3.cc,v 1.1 2007/11/21 16:37:48 bjoo Exp $
/*! \file
 * \brief Main code for spectrum measurements
 */

#include "chroma.h"

using namespace QDP;
using namespace Chroma;


/*
 * Input 
 */
// Parameters which must be determined from the XML input
// and written to the XML output
struct Param_t
{
  multi1d<int> nrow;
};


//! Propagators
struct Prop_t
{
  std::string prop_file;  // The files are expected to be in SciDAC format!
};


//! Mega-structure of parameters
struct App_input_t
{
  Param_t          param;
  Cfg_t            cfg;
  Prop_t           prop;
};


//! XML READ THE PROP STRUCT
void read(XMLReader& xml, const string& path, Prop_t& input)
{
  XMLReader inputtop(xml, path);
  read(inputtop, "prop_file", input.prop_file);
}


//! XML READ THE PARAM STRUCT
void read(XMLReader& xml, const string& path, Param_t& param)
{
  XMLReader paramtop(xml, path);
  read(paramtop, "nrow", param.nrow);
}

// Cfg reader already defined in param/IO

// Reader recursively all the input Param, Cfg_t, Prop
void read(XMLReader& xml, const string& path, App_input_t& input)
{
  XMLReader inputtop(xml, path);

  // Read all the input groups
  try
  {
    // Read program parameters
    read(inputtop, "Param", input.param);

    // Read in the gauge configuration info
    read(inputtop, "Cfg", input.cfg);

    // Read in the propagator(s) info
    read(inputtop, "Prop", input.prop);
  }
  catch (const string& e)  // Catch any exceptions/errors
  {
    QDPIO::cerr << "Error reading prop data: " << e << endl;
    QDP_abort(1);
  }
}


//
// Main program
//
int main(int argc, char **argv)
{
  // Put the machine into a known state
  Chroma::initialize(&argc, &argv);

  // Put this in to enable profiling etc.
  START_CODE();

  // Input parameter structure
  App_input_t  input;

  // Instantiate xml reader for DATA
  // if you used -i input file, Chroma::getXMLInputFileName() returns
  //  the filename you specified
  // otherwise it will return "DATA"
  XMLReader xml_in;
  try { 
    xml_in.open(Chroma::getXMLInputFileName());
  }
  catch(...) { // Catch ALL Exceptions
    QDPIO::cerr << "Could not open input XML file: " 
		<< Chroma::getXMLInputFileName() 
		<< endl;
    QDP_abort(1);  // Semi Graceful exit.
  }

  // Terminal I/O to stdout.
  QDPIO::cout << "Hello World" << endl;

  // Read data: Call top level reader, fill out the structure.
  // Root tag must be <tutorial2>
  read(xml_in, "/tutorial3", input);

  // Set lattice size, shape, etc.
  // using the nrow param from the Prameter structure
  Layout::setLattSize(input.param.nrow);

  // Initialise
  Layout::create();
  
  // -------------------------------------------------------------------
  // Read in the configuration along with relevant information.
  // -------------------------------------------------------------------

  // The gauge field may contain some file XML and other 
  // information (gauge XML): eg in the future the ILDG header(?)
  // We create empty XML readers for this, the gauge reading routine
  // will fill them in
  XMLReader gauge_file_xml, gauge_xml;

  // The gauge field itself
  multi1d<LatticeColorMatrix> u(Nd);
  

  // A convenience routine to read various gauge field formats
  gaugeStartup(gauge_file_xml, gauge_xml, u, input.cfg);

  // Check if the gauge field configuration is unitarized
  unitarityCheck(u);
  // ---------------- Gauge Reading Done -------------------------------


  // ---------------- Read in a propagator -----------------------------

  // Propagator also contains headers: a file XML and a record XML
  // as well as the binary data -- as dictated by SciDAC standards
  // the propagator code now only dumps SciDAC props.
  XMLReader prop_file_xml, prop_record_xml;

  // The propagator itself
  LatticePropagator quark_propagator;
 
  QDPFileReader prop_reader(prop_file_xml,
			    input.prop.prop_file,     
			    QDPIO_SERIAL);

  prop_reader.read(prop_record_xml, quark_propagator);

  prop_reader.close();

  // --------------- Propagator reader done ---------------------------

  // Instantiate XML writer for XMLDAT
  // Chroma::getXMLOutputInstance() returns an XMLFileWriter reference
  // If you gave chroma the command line option:
  //  -o output_xml_file   then output_xml_file is opened 
  //  if you gave no command line option then "XMLDAT" is opened
  XMLFileWriter& xml_out = Chroma::getXMLOutputInstance();

  // Enter group under tag <tutorial3>. This is the first push() making
  // this the root tag.
  push(xml_out, "tutorial3");

  proginfo(xml_out);    // Print out basic program info (writes own 
                        // surrounding tags.

  // Write out the output: 
  //  Note: you can write out the contents of an entire reader.
  //  Here the contents of the reader will go in a group surrounded
  //  with tags <Input> </Input>

  //  If you don't want extra surrounding tags you can use
  //  xml_out << xml_in
  write(xml_out, "Input", xml_in);

  // Write out the config info (same deal as above)
  write(xml_out, "Config_info", gauge_xml);

  // Exercise: Output things about the prop (from prop_record_xml)
  // Here



  // Flush buffers in case you get an error
  xml_out.flush();

  // Calculate the plaquette. This form will write it out
  // information surrounded by tag: <Plaquette> ... </Plaquette>
  MesPlq(xml_out, "Plaquette", u);
  xml_out.flush();


  // Do something exciting here yourself.
  // Suggested exercise: Compute the zero mom pion on the propagator you 
  //                     have read.

  

  // Do something exciting here yourself
  // Suggested exercise: Create a linear operator and apply it
  // to a gaussian noise vector.

  
  // 
  // Write it out in a tag called <Pion>
  // A code snippet for doing this can be found in
  //   chroma/mainprogs/main/propagator.cc be


  pop(xml_out); // Exit the tutorial3 tag

  END_CODE(); // End optimisations

  // Time to bolt
  Chroma::finalize();

  exit(0);
}

#include <iostream>
#include "Moat.hpp"

int main(int argc, const char *argv[])
{
  if (argc < 2)
  {
    cerr << "\n";
    cerr << "Not enough parameters passed. Use -h optional flag for help.\n"
         << endl;
    return 1;
  }

  bool vertical = false;
  bool segmented = false;
  bool verbose = false;
  bool printPrimes = false;

  double jumpSize = 0;
  uint64_t realPart = 0;

  for (int i = 1; i < argc; i++)
  {
    string arg = argv[i];

    // Usage information.
    if (arg == "-h" || arg == "--help")
    {
      cerr << "\n";
      cerr << "Usage: " << argv[0] << " jumpSize [realPart] [option1] [option2] ...\n"
           << "Explore the connected component at the origin in the Gaussian moat problem.\n"
           << "    jumpSize            The jump bound giving adjacency relation among primes.\n"
           << "    realPart            The real-part of the vertical strip to explore if in\n"
           << "                        vertical mode.\n\n"
           << "Exploration modes:\n"
           << "    --origin            Explore the connected component starting at the origin until\n"
           << "                        an impassable moat is encountered. Default exploration mode.\n"
           << "    --segmented         Use a segmented approach to explore the connected component.\n"
           << "                        Algorithm is similar to that in Tsuchimura paper. This approach\n"
           << "                        only counts the size of the component.\n"
           << "    --vertical          Search for a Gaussian moat along a thin vertical strip starting\n"
           << "                        at real-part x. Used to show a component is finite.\n\n"
           << "Options:\n"
           << "    -h, --help          Print this help message.\n"
           << "    -v, --verbose       Display progress.\n"
           << "    -p, --printprimes   Print the real and imag part of primes in the connected component\n"
           << "                        if in origin mode."
           << endl;
      return 1;
    }

    // Getting flags
    if (arg == "-v" || arg == "--verbose")
    {
      verbose = true;
    }
    if (arg == "-p" || arg == "--printprimes")
    {
      printPrimes = true;
    }
    if (arg == "--segmented")
    {
      segmented = true;
    }
    if (arg == "--vertical")
    {
      vertical = true;
    }

    // Parsing for numerical input.
    if (isdigit(arg.front()))
    {
      if (jumpSize == 0.0)
      {
        jumpSize = stod(arg);
      }
      else
        realPart = stoull(arg);
    }
  }

  if (verbose)
  {
    cerr << '\n'
         << endl;
  }
  if (jumpSize == 0.0)
  { // If jumpSize hasn't been parsed, abort.
    cerr << "\nCannot understand input. Use -h optional flag for help.\n"
         << endl;
    return 1;
  }

  if (vertical)
  {
    if (realPart == 0)
    {
      cerr << "\nCannot understand input. Use -h optional flag for help.\n"
           << endl;
      return 1;
    }
    if (verbose)
    {
      cerr << "Searching for moat in vertical strip..." << endl;
    }
    VerticalMoat::setStatics(realPart, jumpSize, verbose);
    VerticalMoat::findVerticalMoat();
  }
  else if (segmented)
  {
    if (verbose)
    {
      cerr << "Searching for moat in segments starting at origin..." << endl;
    }
    SegmentedMoat::setStatics(jumpSize, verbose);
    uint64_t s = SegmentedMoat::getCountMainComponent();
    cerr << "\n\nThe main connected component has size: " << s << endl;
  }
  else
  {
    if (verbose)
    {
      cerr << "Searching for moat starting at origin..." << endl;
    }
    OctantMoat m(jumpSize, 0, verbose);
    m.exploreComponent(0, 0);
    if (printPrimes)
    {
      m.printCurrentComponent();
    }
    cerr << "The main connected component has size: " << m.getComponentSize() << endl;
    gint g = m.getComponentMaxElement();
    cerr << "The farthest out prime in component has coordinates: " << g.a << " " << g.b << endl;
  }

  return 0;
}

/*! \file 
 * **The main() function**
 */

#include "shiba.h"

//! The main program controller


int main(int argc, char **argv)
{

  phylo p;
  // Set defaults for global variables
  PhyloToUse = 0;
  DataFile = "shibaInput.xml";
  PrintData = 0;
  RUN_BATCH = 1000;
  Cfg.verbose = 0;
  Cfg.probSurvA = -1.0;
  Cfg.probDispA = -1.0; 

  // Read arguments
  readArgs(argc, argv);
  // Read datafile
  readXML();

  // Parse phylogeny
  if ((PhyloToUse >= 0) && (PhyloToUse < Phylos))
    p = parseNewick(Phylo[PhyloToUse]);
  else error("Specified phylo number does not match a phylo in input file");
  phyloToLineage(p);

  if (PrintData) printIndata();

  // main runs
  shiba(p);

  // Free memory
  free(p.parent);
  free(p.ndaughter);
  free(p.depth);
  free(p.age);
  free(p.bl);
  free2d1_c(p.taxon, p.nnodes);
  free2d_d(Area, Times);
  free3d_d(Dist, Times, Spaces);
  free(RealTime);
  free2d1_c(SpaceName, Spaces);
  free2d1_c(Phylo, Phylos);
  free2d1_c(Taxon, Taxa);
  free3d_i(Extant, Taxa, Times);
  free(Cfg.startSpace);
  free2d_i(LineagePeriod, p.nnodes);
  free(LineageDaughtersN);
  free2d1_i(LineageDaughters, p.nnodes);
  free3d_i(LineageExtant, Lineages, Times);

  return 1;
}

/*! Parses arguments to the executable. Pass argc and argv directly from main()
 *  . copied from the GNU libc `getopt()`
 * [example](http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html).
 * \param argc The number of arguments
 * \param argv The array of strings of the various arguments
 */

void readArgs(int argc, char **argv)
{
  int c;
  opterr = 0;
     
  while ((c = getopt (argc, argv, "f:hp:lvd:s:")) != -1)
    switch (c)
      {
      case 'f':
        DataFile = optarg;
        break;
      case 'l':
        PrintData = 1;
        break;
      case 'p':
        PhyloToUse = atoi(optarg);
        break;
      case 'd':
        if ((atof(optarg) > 0.0) && (atof(optarg) <= 1.0)) {
          Cfg.probDispA = atof(optarg);
          break;
        }
        else error("Dispersal parameter on command line not acceptable"); 
      case 's':
        if ((atof(optarg) > 0.0) && (atof(optarg) <= 1.0)) {
          Cfg.probSurvA = atof(optarg);
          break;
        }
        else error("Survival parameter on command line not acceptable"); 
      case 'v':
        RUN_BATCH = 1; // cannot do verbose in parallel mode
        Cfg.verbose = 1;
        break;
      case 'h':
        help();
      case '?':
        if (optopt == 'f')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
      default:
        abort();
      }
}

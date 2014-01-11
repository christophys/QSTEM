/*
  QSTEM - image simulation for TEM/STEM/CBED
  Copyright (C) 2000-2010  Christoph Koch
  Copyright (C) 2010-2013  Christoph Koch, Michael Sarahan
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "stemtypes_fftw3.hpp"
#include "config_readers.hpp"
#include "structure_readers.hpp"
#include <string>
#include <map>

#include <boost/filesystem.hpp>

class CCrystal
{
public:
  CCrystal(ConfigReaderPtr &configReader);
  CCrystal(unsigned ncx, unsigned ncy, unsigned ncz,    // ncells in any given direction
	  float_tt tx, float_tt ty, float_tt tz				// Tilts in any given direction
	  );
  ~CCrystal();
  
  void Init(unsigned run_number);
  void ReadUnitCell(bool handleVacancies);
  void TiltBoxed(int ncoord,bool handleVacancies);
  void PhononDisplacement(float_tt *u,int id,int icx,int icy,
                          int icz,atom &atom,bool printReport);
  void ReplicateUnitCell(int handleVacancies);
  void WriteStructure(unsigned run_number);

  void DisplayParams();

  void SetCellParameters(float_tt ax, float_tt by, float_tt cz);

  float_tt GetCZ(){return m_cz;}
  bool GetTDS(){return m_tds;}
  void GetCellParameters(float_tt &ax, float_tt &by, float_tt &cz);
  unsigned GetZnum(unsigned idx){return m_Znums[idx];}
  std::vector<unsigned> GetAtomTypes(){return m_Znums;}
  unsigned GetNumberOfAtomTypes(){return m_Znums.size();}
  float_tt GetU2(unsigned znum){return m_u2[znum];}
  float_tt GetU2avg(unsigned znum){return m_u2avg[znum];}
  
protected:
	boost::filesystem::path m_structureFile;

  std::vector<atom> m_atoms; // The atoms after duplication, tilt, and phonon shaking
  std::vector<atom> m_baseAtoms; // The atoms read directly from the input file (no alteration)
  float_tt **m_Mm;                     /* metric matrix Mm(ax,by,cz,alpha,beta,gamma) */
  float_tt m_ax, m_by, m_cz;           /* lattice parameters */
  float_tt m_cAlpha, m_cBeta, m_cGamma;
  float_tt m_cubex, m_cubey, m_cubez;  /* dimension of crystal cube, if zero, then nx,ny,nz *
  //					 * will be used */
  bool m_adjustCubeSize;  
  float_tt m_offsetX, m_offsetY;
  float_tt m_ctiltx, m_ctilty, m_ctiltz;  /* crystal tilt in mrad */
  unsigned m_nCellX, m_nCellY, m_nCellZ;  /* number of unit cells in x-y-z dir*/
  StructureReaderPtr m_reader;

  std::vector<unsigned> m_Znums; // Z numbers for the atom types that are present
  std::map<unsigned, float_tt> m_u2, m_u2avg;  /* (current/averaged) rms displacement of atoms */

  boost::filesystem::path m_phononFile;

  StructureReaderPtr m_structureReader;
  StructureWriterPtr m_structureWriter;
  
  bool m_tds; // if set, TDS will be applied to atom positions
  bool m_Einstein; /* if set (default=set), the Einstein model will be used */
  float_tt m_tds_temp;  // The temperature for TDS calculations

  int m_printLevel;

  void CalculateCellDimensions();

  void OffsetCenter(atom &center);

  void Inverse_3x3 (float_tt *res, const float_tt *a);
  void RotateVect(float_tt *vectIn,float_tt *vectOut, float_tt phi_x, float_tt phi_y, float_tt phi_z);
  void MatrixProduct(float_tt **a,int Nxa, int Nya, float_tt **b,int Nxb, int Nyb, float_tt **c);
  void RotateMatrix(float_tt *matrixIn,float_tt *matrixOut, float_tt phi_x, float_tt phi_y, float_tt phi_z);

  static int AtomCompareZnum(const void *atPtr1,const void *atPtr2);
  static int AtomCompareZYX(const void *atPtr1,const void *atPtr2);
};

typedef boost::shared_ptr<CCrystal> StructurePtr;

#endif











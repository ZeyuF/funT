/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
#ifndef SLR_COMPONENT_H
#define SLR_COMPONENT_H
/*
 *  slr_component.hpp
 *  hector
 *
 *  Created by Ben on 31 January 2012.
 *
 */

#include "imodel_component.hpp"
#include "logger.hpp"
#include "tseries.hpp"
#include "unitval.hpp"

// Need to forward declare the components which depend on each other
#include "temperature_component.hpp"

namespace Hector {

//------------------------------------------------------------------------------
/*! \brief The sea level rise component.
 *
 *  Computes sea level rise from mean global temperature, based on
 *  Vermeer, M. and S. Rahmstorf (2009) in PNAS.
 */
class slrComponent : public IModelComponent {

public:
  slrComponent();
  ~slrComponent();

  //! IModelComponent methods
  virtual std::string getComponentName() const;

  virtual void init(Core *core);

  virtual unitval sendMessage(const std::string &message,
                              const std::string &datum,
                              const message_data info = message_data());

  virtual void setData(const std::string &varName, const message_data &);

  virtual void prepareToRun();

  virtual void run(const double runToDate);

  virtual void reset(double time);

  virtual void shutDown();

  //! IVisitable methods
  virtual void accept(AVisitor *visitor);

  //! Reference period. No output occurs before we reach refperiod_high
  int refperiod_low, refperiod_high;
  int normalize_year;

private:
  virtual unitval getData(const std::string &varName, const double valueIndex);

  tseries<unitval> sl_rc;        //!< sea level rate of change, cm/yr
  tseries<unitval> slr;          //!< sea level rise, cm
  tseries<unitval> sl_rc_no_ice; //!< sea level rate of change, cm/yr, no ice
  tseries<unitval> slr_no_ice;   //!< sea level rise, cm, no ice

  unitval refperiod_tgav; //!< reference period mean temperature
  tseries<unitval> tgav;  //!< private copy of global mean temperature

  //! pointers to other components and stuff
  Core *core;

  void compute_slr(const double date);

  //! logger
  Logger logger;

  //! Last date run to
  double oldDate;
};

} // namespace Hector

#endif // SLR_COMPONENT_H

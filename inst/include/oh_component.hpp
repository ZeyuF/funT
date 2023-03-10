/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/

#ifndef OH_COMPONENT_H
#define OH_COMPONENT_H
/*
 *  oh_component.hpp
 *  hector
 *
 *  Created by Corinne on 11/20/2014.
 *
 */

#include "imodel_component.hpp"
#include "logger.hpp"
#include "tseries.hpp"
#include "unitval.hpp"

namespace Hector {

//------------------------------------------------------------------------------
/*! \brief Methane model component.
 *
 *  This doesn't do much yet.
 */
class OHComponent : public IModelComponent {

public:
  OHComponent();
  ~OHComponent();

  // IModelComponent methods
  virtual std::string getComponentName() const;

  virtual void init(Core *core);

  virtual unitval sendMessage(const std::string &message,
                              const std::string &datum,
                              const message_data info = message_data());

  virtual void setData(const std::string &varName, const message_data &data);

  virtual void prepareToRun();

  virtual void run(const double runToDate);

  virtual void reset(double date);

  virtual void shutDown();

  // IVisitable methods
  virtual void accept(AVisitor *visitor);

private:
  virtual unitval getData(const std::string &varName, const double date);
  //! emissions time series
  tseries<unitval> CO_emissions;
  tseries<unitval> NOX_emissions;
  tseries<unitval> NMVOC_emissions;
  tseries<unitval> TAU_OH;

  unitval M0;   // initial CH4 concentration
  unitval TOH0; // preindustrial OH lifetime

  double CCO;    // coefficent for CO
  double CNMVOC; // coefficent for NMVOC
  double CNOX;   // coefficent for NOX
  double CCH4;   // coefficent for CH4

  // logger
  Logger logger;

  Core *core;
  double oldDate;
};

} // namespace Hector

#endif // OH_COMPONENT_H

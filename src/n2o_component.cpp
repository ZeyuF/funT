/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
/*
 *  n2o_component.cpp
 *  hector
 *
 *  Created by Ben on 05/19/2011.
 *
 */

#include "n2o_component.hpp"
#include "avisitor.hpp"
#include "core.hpp"
#include "h_util.hpp"
#include <math.h>

namespace Hector {

using namespace std;

//------------------------------------------------------------------------------
/*! \brief Constructor
 */
N2OComponent::N2OComponent() {
  N2O_emissions.allowInterp(true);
  N2O_emissions.name = N2O_COMPONENT_NAME;
  N2O_natural_emissions.allowInterp(true);
  N2O_natural_emissions.name = D_NAT_EMISSIONS_N2O;
  N2O.allowInterp(true);
  N2O.name = D_N2O_CONC;
  TAU_N2O.allowInterp(true);
  TAU_N2O.name = D_TAU_N2O;
}

//------------------------------------------------------------------------------
/*! \brief Destructor
 */
N2OComponent::~N2OComponent() {}

//------------------------------------------------------------------------------
// documentation is inherited
string N2OComponent::getComponentName() const {
  const string name = N2O_COMPONENT_NAME;

  return name;
}

//------------------------------------------------------------------------------
// documentation is inherited
void N2OComponent::init(Core *coreptr) {
  logger.open(getComponentName(), false,
              coreptr->getGlobalLogger().getEchoToFile(),
              coreptr->getGlobalLogger().getMinLogLevel());
  H_LOG(logger, Logger::DEBUG) << "hello " << getComponentName() << std::endl;
  core = coreptr;
  oldDate = core->getStartDate();

  // Inform core what data we can provide
  core->registerCapability(D_N2O_CONC, getComponentName());
  core->registerCapability(D_PREINDUSTRIAL_N2O, getComponentName());
  core->registerCapability(D_EMISSIONS_N2O, getComponentName());
  core->registerCapability(D_CONSTRAINT_N2O, getComponentName());
  // register data we can accept as input
  core->registerInput(D_EMISSIONS_N2O, getComponentName());
  core->registerInput(D_PREINDUSTRIAL_N2O, getComponentName());
  core->registerInput(D_CONSTRAINT_N2O, getComponentName());
  core->registerInput(D_NAT_EMISSIONS_N2O, getComponentName());
}

//------------------------------------------------------------------------------
// documentation is inherited
unitval N2OComponent::sendMessage(const std::string &message,
                                  const std::string &datum,
                                  const message_data info) {
  unitval returnval;

  if (message == M_GETDATA) { //! Caller is requesting data
    return getData(datum, info.date);

  } else if (message == M_SETDATA) { //! Caller is requesting to set data
    setData(datum, info);

  } else { //! We don't handle any other messages
    H_THROW("Caller sent unknown message: " + message);
  }

  return returnval;
}

//------------------------------------------------------------------------------
// documentation is inherited
void N2OComponent::setData(const string &varName, const message_data &data) {
  try {
    if (varName == D_PREINDUSTRIAL_N2O) {
      H_ASSERT(data.date == Core::undefinedIndex(), "date not allowed");
      if (data.getUnitval(U_PPBV_N2O).value(U_PPBV_N2O) != 273.87) {
        H_LOG(logger, Logger::WARNING)
            << "Changing " << varName
            << " from default value; this is not recomended and may cause"
            << " issues with RF N2O calucation" << std::endl;
      }
      N0 = data.getUnitval(U_PPBV_N2O);
    } else if (varName == D_EMISSIONS_N2O) {
      H_ASSERT(data.date != Core::undefinedIndex(), "date required");
      N2O_emissions.set(data.date, data.getUnitval(U_TG_N));
    } else if (varName == D_NAT_EMISSIONS_N2O) {
      H_ASSERT(data.date != Core::undefinedIndex(), "date required");
      N2O_natural_emissions.set(data.date, data.getUnitval(U_TG_N));
    } else if (varName == D_CONVERSION_N2O) {
      H_ASSERT(data.date == Core::undefinedIndex(), "date not allowed");
      UC_N2O = data.getUnitval(U_TG_PPBV);
    } else if (varName == D_INITIAL_LIFETIME_N2O) {
      H_ASSERT(data.date == Core::undefinedIndex(), "date not allowed");
      TN2O0 = data.getUnitval(U_YRS);
    } else if (varName == D_N2O_CONC) {
      H_ASSERT(data.date != Core::undefinedIndex(), "date required");
      N2O.set(data.date, data.getUnitval(U_PPBV_N2O));
    } else if (varName == D_CONSTRAINT_N2O) {
      H_ASSERT(data.date != Core::undefinedIndex(), "date required");
      N2O_constrain.set(data.date, data.getUnitval(U_PPBV_N2O));
    } else {
      H_THROW("Unknown variable name while parsing " + getComponentName() +
              ": " + varName);
    }
  } catch (h_exception &parseException) {
    H_RETHROW(parseException, "Could not parse var: " + varName);
  }
}

//------------------------------------------------------------------------------
// documentation is inherited
void N2OComponent::prepareToRun() {

  H_LOG(logger, Logger::DEBUG) << "prepareToRun " << std::endl;
  oldDate = core->getStartDate();

  if (N2O_constrain.size() && N2O_constrain.exists(oldDate)) {
    H_LOG(logger, Logger::WARNING)
        << "Overwriting preindustrial N2O value with N2O constraint value"
        << std::endl;
    N0 = N2O_constrain.get(oldDate);
  }
  N2O.set(oldDate, N0);
}

//------------------------------------------------------------------------------
// documentation is inherited
void N2OComponent::run(const double runToDate) {

  H_ASSERT(!core->inSpinup() && runToDate - oldDate == 1,
           "timestep must equal 1");

  if (N2O_constrain.size() && N2O_constrain.exists(runToDate)) {
    N2O.set(runToDate, N2O_constrain.get(runToDate));
  } else {
    // Approach modified from Ward and Mahowald, 2014, 10.5194/acp-14-12701-2014
    double previous_n2o = N0.value(U_PPBV_N2O);

    if (runToDate != oldDate) {
      previous_n2o = N2O.get(oldDate);
    }

    // Decay constant varies based on N2O concentrations
    // This is Eq. B8 in Ward and Mahowald, 2014
    TAU_N2O.set(runToDate,
                unitval(TN2O0.value(U_YRS) *
                            (pow(previous_n2o / N0.value(U_PPBV_N2O), -0.05)),
                        U_YRS));

    // Current emissions are the sum of natural and anthropogenic sources
    const double current_n2oem =
        N2O_emissions.get(runToDate).value(U_TG_N) +
        N2O_natural_emissions.get(runToDate).value(U_TG_N);

    // This calculation follows Eq. B7 in Ward and Mahowald 2014
    const double dN2O = current_n2oem / UC_N2O -
                        previous_n2o / TAU_N2O.get(runToDate).value(U_YRS);

    N2O.set(runToDate, unitval(previous_n2o + dN2O, U_PPBV_N2O));
    H_LOG(logger, Logger::DEBUG)
        << runToDate << "tau = " << TAU_N2O.get(runToDate);
  }

  oldDate = runToDate;
  H_LOG(logger, Logger::DEBUG)
      << runToDate << " N2O = " << N2O.get(runToDate) << std::endl;
}

//------------------------------------------------------------------------------
// documentation is inherited
unitval N2OComponent::getData(const std::string &varName, const double date) {

  unitval returnval;

  if (varName == D_N2O_CONC) {
    H_ASSERT(date != Core::undefinedIndex(),
             "Date required for atmospheric N2O");
    returnval = N2O.get(date);
  } else if (varName == D_PREINDUSTRIAL_N2O) {
    H_ASSERT(date == Core::undefinedIndex(),
             "Date not allowed for preindustrial N2O");
    returnval = N0;
  } else if (varName == D_EMISSIONS_N2O) {
    H_ASSERT(date != Core::undefinedIndex(), "Date required for N2O emissions");
    returnval = N2O_emissions.get(date);
  } else if (varName == D_NAT_EMISSIONS_N2O) {
    H_ASSERT(date != Core::undefinedIndex(),
             "Date required for natural N2O emissions");
    returnval = N2O_natural_emissions.get(date);
  } else if (varName == D_CONSTRAINT_N2O) {
    H_ASSERT(date != Core::undefinedIndex(),
             "Date required for N2O constraint");
    if (N2O_constrain.exists(date)) {
      returnval = N2O_constrain.get(date);
    } else {
      H_LOG(logger, Logger::DEBUG)
          << "No N2O constraint for requested date " << date
          << ". Returning missing value." << std::endl;
      returnval = unitval(MISSING_FLOAT, U_PPBV_N2O);
    }
  } else {
    H_THROW("Caller is requesting unknown variable: " + varName);
  }

  return returnval;
}

//------------------------------------------------------------------------------
// documentation is inherited
void N2OComponent::reset(double time) {
  // reset time counter, and truncate output time series
  oldDate = time;
  N2O.truncate(time);
  TAU_N2O.truncate(time);
  H_LOG(logger, Logger::NOTICE)
      << getComponentName() << " reset to time= " << time << "\n";
}

//------------------------------------------------------------------------------
// documentation is inherited
void N2OComponent::shutDown() {
  H_LOG(logger, Logger::DEBUG) << "goodbye " << getComponentName() << std::endl;
  logger.close();
}

//------------------------------------------------------------------------------
// documentation is inherited
void N2OComponent::accept(AVisitor *visitor) { visitor->visit(this); }

} // namespace Hector

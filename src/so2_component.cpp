/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
/*
 *  so2_component.cpp
 *  hector
 *
 *  Created by Corinne on 5/6/2013
 *
 *  Note that the SO2 emissions are read in terms of S instead of SO2
 *  values may need to be converted from g SO2 to g of S.
 *
 */

#include "so2_component.hpp"
#include "avisitor.hpp"
#include "core.hpp"
#include "h_util.hpp"

namespace Hector {

using namespace std;

//------------------------------------------------------------------------------
/*! \brief Constructor
 */
SulfurComponent::SulfurComponent() {
  SO2_emissions.allowInterp(true);
  SV.allowInterp(true);
  SO2_emissions.name = SULFUR_COMPONENT_NAME;
  SV.name = SULFUR_COMPONENT_NAME;
}

//------------------------------------------------------------------------------
/*! \brief Destructor
 */
SulfurComponent::~SulfurComponent() {}

//------------------------------------------------------------------------------
// documentation is inherited
string SulfurComponent::getComponentName() const {
  const string name = SULFUR_COMPONENT_NAME;

  return name;
}

//------------------------------------------------------------------------------
// documentation is inherited
void SulfurComponent::init(Core *coreptr) {
  logger.open(getComponentName(), false,
              coreptr->getGlobalLogger().getEchoToFile(),
              coreptr->getGlobalLogger().getMinLogLevel());
  H_LOG(logger, Logger::DEBUG) << "hello " << getComponentName() << std::endl;
  core = coreptr;

  // Inform core what data we can provide
  core->registerCapability(D_EMISSIONS_SO2, getComponentName());
  core->registerCapability(D_VOLCANIC_SO2, getComponentName());

  // accept anthro emissions, volcanic, and natural emissions as inputs
  core->registerInput(D_EMISSIONS_SO2, getComponentName());
  core->registerInput(D_VOLCANIC_SO2, getComponentName());
}

//------------------------------------------------------------------------------
// documentation is inherited
unitval SulfurComponent::sendMessage(const std::string &message,
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
void SulfurComponent::setData(const string &varName, const message_data &data) {
  H_LOG(logger, Logger::DEBUG) << "Setting " << varName << "[" << data.date
                               << "]=" << data.value_str << std::endl;

  try {
    if (varName == D_EMISSIONS_SO2) {
      H_ASSERT(data.date != Core::undefinedIndex(), "date required");
      SO2_emissions.set(data.date, data.getUnitval(U_GG_S));
    } else if (varName == D_VOLCANIC_SO2) {
      H_ASSERT(data.date != Core::undefinedIndex(), "date required");
      SV.set(data.date, data.getUnitval(U_W_M2));
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
void SulfurComponent::prepareToRun() {

  H_LOG(logger, Logger::DEBUG) << "prepareToRun " << std::endl;
  oldDate = core->getStartDate();
}

//------------------------------------------------------------------------------
// documentation is inherited
void SulfurComponent::run(const double runToDate) {
  H_ASSERT(!core->inSpinup() && runToDate - oldDate == 1,
           "timestep must equal 1");
  oldDate = runToDate;
}

//------------------------------------------------------------------------------
// documentation is inherited
unitval SulfurComponent::getData(const std::string &varName,
                                 const double date) {

  unitval returnval;

  if (varName == D_EMISSIONS_SO2) {
    H_ASSERT(date != Core::undefinedIndex(), "Date required for SO2 emissions");
    returnval = SO2_emissions.get(date);
  } else if (varName == D_VOLCANIC_SO2) {
    H_ASSERT(date != Core::undefinedIndex(), "Date required for volcanic SO2");
    if (SV.size())
      returnval = SV.get(date);
    else
      returnval = unitval(0.0, U_W_M2);
  } else {
    H_THROW("Caller is requesting unknown variable: " + varName);
  }

  return returnval;
}

//------------------------------------------------------------------------------
// documentation is inherited
void SulfurComponent::reset(double time) {
  // This component doesn't calculate anything, so all we have to do
  // is reset the time counter.
  oldDate = time;
  H_LOG(logger, Logger::NOTICE)
      << getComponentName() << " reset to time= " << time << "\n";
}

//------------------------------------------------------------------------------
// documentation is inherited
void SulfurComponent::shutDown() {
  H_LOG(logger, Logger::DEBUG) << "goodbye " << getComponentName() << std::endl;
  logger.close();
}

//------------------------------------------------------------------------------
// documentation is inherited
void SulfurComponent::accept(AVisitor *visitor) { visitor->visit(this); }

} // namespace Hector

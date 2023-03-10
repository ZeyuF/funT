#### Documentation objects
#' Hector logging levels
#'
#' These functions return constants that can be used in
#' \code{\link{newcore}} to set the minimum logging level
#'
#' @name loglevels
NULL

#' Utility functions for Hector instances
#'
#' @param core Hector instance to operate on
#' @name hectorutil
NULL

#' Message type identifiers
#'
#' These identifiers indicate the type of a message so that the core can route
#' it to the correct method in the recipient.
#'
#' @section Note:
#' Because these identifiers are provided as \code{#define} macros in the hector code,
#' these identifiers are provided in the R interface as functions. Therefore,
#' these objects must be called to use them; \emph{e.g.}, \code{GETDATA()}
#' instead of the more natural looking \code{GETDATA}.
#'
#' @name msgtype
NULL

#' Identifiers for capabilities in the Hector forcing component
#'
#' These identifiers specify forcing values that can be provided by hector via
#' the forcing component. All of the values corresponding to these identifiers
#' are read-only (\emph{i.e.}, they can only appear in \code{\link{GETDATA}}
#' messages.)
#'
#' @inheritSection msgtype Note
#'
#' @name forcings
#' @seealso \link{haloforcings} for forcings from halocarbons and \link{rho} for
#' parameters related to radiative forcing efficiency.
#' @family capability identifiers
NULL

#' Identifiers for Hector forcing component parameters
#'
#' These identifiers specify radiative forcing efficiency in hector these values
#' can be read and/or set by hectors forcing and halocarbon components.
#' the forcing component.
#'
#' @inheritSection msgtype Note
#'
#' @name rho
#' @seealso \link{haloforcings} for forcings from halocarbons and \link{forcings} forcing
#' values provided from the hector forcing component.
#' @family capability identifiers
NULL

#' Identifiers for Hector forcing component parameters
#'
#' These identifiers specify the tropospheric adjustments for the
#' stratospheric-temperature adjusted radiative forcings.
#' These values must be a number between -1 and 1, and
#' can be read and/or set by hectors forcing and halocarbon components.
#'
#' @inheritSection msgtype Note
#'
#' @name delta
#' @seealso \link{haloforcings} for forcings from halocarbons and \link{forcings} forcing
#' values provided from the hector forcing component.
#' @family capability identifiers
NULL

#' Identifiers for variables in the Hector carbon cycle component
#'
#' These identifiers correspond to variables that can be read and/or set in the
#' carbon cycle
#'
#' @section Output variables:
#' These variables can be read using the \code{\link{GETDATA}} message type:
#' \describe{
#' \item{CONCENTRATIONS_CO2}{Atmospheric CO2 concentration}
#' \item{ATMOSPHERIC_CO2}{Atmospheric CO2 in units of C}
#' \item{NBP}{Net biome production. Annual global C flux from atmosphere into
#'   the land. A positive value means a net flux from atmosphere into land
#'   (i.e. land is a net carbon sink), while a negative value means a net flux
#'   from land into the atmosphere (i.e. land is a net carbon source). Note
#'   that despite the name--which follows disciplinary convention, see Chapin
#'   et al. 2006--this is currently a globally-averaged variable.}
#' \item{FFI_EMISSIONS}{Fossil fuel and industrial emissions}
#' \item{LUC_EMISSIONS}{Land use change emissions}
#' \item{NPP}{Net primary production}
#' \item{RH}{Heterotrophic respiration}
#' \item{EARTH_C}{Earth pool}
#' }
#' @section Input variables:
#' \describe{
#' \item{FFI_EMISSIONS}{Fossil fuel and industrial emissions (\code{"Pg C/yr"})}
#' \item{LUC_EMISSIONS}{Land use change emissions (\code{"Pg C/yr"})}
#' \item{CO2_CONSTRAIN}{Prescribed atmospheric CO2 concentration (\code{"ppmv CO2"})}
#' \item{NBP_CONSTRAIN}{Prescribed net biome production (land-atmosphere C flux) (\code{"Pg C/yr"})}
#' \item{DACCS_UPTAKE}{Direct air carbon capture and storage}
#' \item{LUC_UPTAKE}{Land use change uptake}
#' }
#' @inheritSection msgtype Note
#' @name carboncycle
#' @seealso \link{concentrations} for other gas concentrations and
#' \link{emissions} for other gas emissions
#' @family capability identifiers
NULL

#' Identifiers for halocarbon forcings
#'
#' These identifiers specify forcing values that can be provided by hector via
#' one of the myriad halocarbon components. All of the values corresponding to
#' these identifiers are read-only (\emph{i.e.}, they can only appear in
#' \code{\link{GETDATA}} messages.)  The forcings returned are the
#' \emph{relative} forcings, with the base year (typically 1750) values
#' subtracted off.
#'
#' @inheritSection msgtype Note
#'
#' @name haloforcings
#' @seealso \link{forcings} for forcings from other sources.
#' @family capability identifiers
NULL

#' Identifiers for halocarbon emissions
#'
#' These identifiers correspond to emissions values for halocarbons. They are
#' (for now) write only, meaning they can only appear in \code{\link{SETDATA}}
#' messages. In all cases, the expected input units are gigagrams
#' (\code{"Gg"}).
#'
#' @inheritSection msgtype Note
#'
#' @name haloemiss
#' @family capability identifiers
NULL

#' Identifiers for halocarbon concentration constraints
#'
#' These identifiers correspond to concentration constraints for halocarbons. In
#' all cases, the expected input units are volumetric parts per trillion
#' (\code{"ppvt"}).
#'
#' @inheritSection msgtype Note
#'
#' @name haloconstrain
#' @family capability identifiers
NULL

#' Identifiers for constraints
#'
#' These identifiers correspond to Hector's constraint capabilities
#'
#' @inheritSection msgtype Note
#'
#' @name constraints
#' @family capability identifiers
#' @seealso haloconstrains
NULL

#' Identifiers for quantities in the methane component
#'
#' These identifiers correspond to variables that can be read and/or set in the
#' methane component.
#'
#' @section Output variables:
#' These variables can be read from the methane component.
#' \describe{
#' \item{CONCENTRATIONS_CH4}{Atmospheric methane concentration}
#' \item{PREINDUSTRIAL_CH4}{Preindustrial methane concentration}
#' }
#'
#'
#' @section Input variables:
#' These variables can be set in the methane component. The expected units
#' string is given after each description.
#' \describe{
#' \item{EMISSIONS_CH4}{Methane emissions (\code{"Tg CH4"})}
#' \item{PREINDUSTRIAL_CH4}{Preindustrial methane concentration (\code{"ppbv CH4"})}
#' \item{CH4_CONSTRAIN}{N2O concentration constraint (\code{"ppbv CH4"})}
#' \item{NATURAL_CH4}{Natural methane emissions (\code{"Tg CH4"})}
#' \item{LIFETIME_SOIL}{Time scale for methane loss into soil (\code{"Years"})}
#' \item{LIFETIME_STRAT}{Time scale for methane loss into stratosphere (\code{"Years"})}
#' }
#'
#' @inheritSection msgtype Note
#'
#' @name methane
#' @family capability identifiers
NULL

#' Identifiers for quantities in the SO2 component
#'
#' These identifiers correspond to variables that can be read and/or set in the
#' SO2 component
#'
#' @section Output variables:
#' These variables can be read using the \code{\link{GETDATA}} message type.
#' \describe{
#' \item{NATURAL_SO2}{Natural SO2 emissions}
#' \item{Y2000_SO2}{Year 2000 SO2 emissions}
#' \item{EMISIONS_SO2}{Anthropogenic SO2 emissions}
#' \item{VOLCANIC_SO2}{Forcing due to volcanic SO2 emissions}
#' }
#'
#' @section Input variables:
#' These variables can be set using the \code{\link{SETDATA}} message type.
#' \describe{
#' \item{EMISSIONS_SO2}{Anthropogenic SO2 emissions (\code{"Gg S"})}
#' \item{VOLCANIC_SO2}{Forcing due to volcanic SO2 emissions (\code{"W/m2"})}
#' }
#'
#' @inheritSection msgtype Note
#'
#' @name so2
#' @family capability identifiers
NULL

#' Identifiers for quantities in the ocean component
#'
#' These identifiers correspond to variables that can be read from the ocean
#' component using the \code{\link{GETDATA}} message type.
#'
#' @inheritSection msgtype Note
#'
#' @name ocean
#' @family capability identifiers
NULL

#' Identifiers for miscellaneous concentrations not elsewhere described
#'
#' All of these variables can be read using the \code{\link{GETDATA}} message
#' type.
#'
#' @inheritSection msgtype Note
#' @name concentrations
#' @family capability identifiers
NULL

#' Identifiers for miscellaneous emissions not elsewhere described
#'
#' All of these variables can be set using the \code{\link{SETDATA}} message
#' type.
#'
#' @inheritSection msgtype Note
#' @name emissions
#' @family capability identifiers
NULL

#' Identifiers for model parameters
#'
#' These identifiers correspond to settable parameters that change the model
#' behavior and are subject to uncertainty. All of these can be set using the
#' \code{\link{SETDATA}} message type. Changing any of these parameters will
#' typically invalidate the hector core's internal state; therefore, after
#' setting one or more of these values you should call \code{\link{reset}} before
#' attempting to run the model again. This will rerun the spinup and produce a
#' new internally consistent state. Attempting to run the model without resetting
#' first will usually produce an error (often with a message about failing to
#' conserve mass).
#'
#' @inheritSection msgtype Note
#' @name parameters
#' @family capability identifiers
NULL

#' Identifiers for variables associated with the temperature component
#'
#' All of these variables can be read using the \code{\link{GETDATA}} message
#' type.
#'
#' @inheritSection msgtype Note
#' @name temperature
#' @family capability identifiers
NULL

#' A data.frame containing information on all of the Hector inputs listed in
#' an ini file created by save-input-params.R.
#'
#' @format A data.frame of 8 columns and 222 rows.
#' \describe{
#'  \item{section}{String of the section of the ini file of a parameter}
#'  \item{parameter}{String of the parameter name}
#'  \item{biome.specific}{String of whether or not the parameter is biome-specific}
#'  \item{time.variant}{String of whether or not the parameter is time-variant}
#'  \item{required}{String of wheterh or not the parameter is required}
#'  \item{default}{String of the default value of the parameter}
#'  \item{units}{String of the parameter units}
#'  \item{description}{String of a brief description of the parameter from the ini file}
#' }
"inputstable"

#' A data.frame containing information on all of the Hector variable created by units-data.R
#' used within the getunits function.
#'
#' @format A data.frame of 2 columns and 114 rows.
#' \describe{
#'  \item{variable}{String of the hector variable}
#'  \item{units}{String of the unit value}
#' }
"unitstable"


#' A data.frame containing information R hector function name and hector string names.
#'
#' @format A data.frame of 2 columns and 239 rows.
#' \describe{
#'  \item{fxn}{String of the R hector function name}
#'  \item{string}{String of variable name}
#' }
"fxntable"

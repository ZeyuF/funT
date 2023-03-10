% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/biome.R
\name{create_biome}
\alias{create_biome}
\title{Create new biome}
\usage{
create_biome(
  core,
  biome,
  veg_c0,
  detritus_c0,
  soil_c0,
  permafrost_c0,
  npp_flux0,
  warmingfactor,
  beta,
  q10_rh,
  f_nppv,
  f_nppd,
  f_litterd
)
}
\arguments{
\item{core}{Hector core}

\item{biome}{Name of new biome}

\item{veg_c0}{Initial vegetation C pool}

\item{detritus_c0}{Initial detritus C pool}

\item{soil_c0}{Initial soil C pool}

\item{permafrost_c0}{Initial permafrost C pool}

\item{npp_flux0}{Initial net primary productivity}

\item{warmingfactor}{Temperature multiplier (default =
`1.0`)}

\item{beta}{CO2 fertilization effect (default = `0.36`)}

\item{q10_rh}{Q10 of heterotrophic respiration (default = `2.0`)}

\item{f_nppv}{Fraction of NPP to vegetation (default = `0.35`)}

\item{f_nppd}{Fraction of NPP to detritus (default = `0.60`)}

\item{f_litterd}{Fraction of litter flux to detritus (default = `0.98`)}
}
\value{
Hector core, invisibly
}
\description{
Create new biome
}
\author{
Alexey Shiklomanov
}

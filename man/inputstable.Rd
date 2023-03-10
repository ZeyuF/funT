% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/aadoc.R
\docType{data}
\name{inputstable}
\alias{inputstable}
\title{A data.frame containing information on all of the Hector inputs listed in
an ini file created by save-input-params.R.}
\format{
A data.frame of 8 columns and 222 rows.
\describe{
 \item{section}{String of the section of the ini file of a parameter}
 \item{parameter}{String of the parameter name}
 \item{biome.specific}{String of whether or not the parameter is biome-specific}
 \item{time.variant}{String of whether or not the parameter is time-variant}
 \item{required}{String of wheterh or not the parameter is required}
 \item{default}{String of the default value of the parameter}
 \item{units}{String of the parameter units}
 \item{description}{String of a brief description of the parameter from the ini file}
}
}
\usage{
inputstable
}
\description{
A data.frame containing information on all of the Hector inputs listed in
an ini file created by save-input-params.R.
}
\keyword{datasets}

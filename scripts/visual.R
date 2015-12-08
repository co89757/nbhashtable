## seriesX is a vector of values for each label in the vector: horizontal_labels
library(gplots)
mybarplot <- function(seriesA, seriesB, seriesC, horizontal_labels,... )
{
  matx <- matrix(c(seriesA,seriesB,seriesC), nrow=length(horizontal_labels),ncol=3) 
  matx <- t(matx)

 ### BARPLOT
  barplot(matx,
          beside=T,
          space=c(0.2,0.8),# Amount of space between i) bars within a group, ii) bars between groups  
          names.arg=horizontal_labels, # names for bars
          col=c("blue","red","yellow"),
          border = "black",
          ...
          )

 legend("topleft",
        legend=c("seriesA","seriesB","seriesC"),
        fill=c("blue","red","yellow") )
  
  
}

seriesA=c(10,20,30)
seriesB=c(5,8,9)
seriesC=c(15,25,45)
xlabels = c("math","science","english")
mybarplot(seriesA,seriesB,seriesC,xlabels,main="sample")
#pdf(filena)

# EXAMPLE FOR BARPLOT: GENERATE THE DATA  
  
# Age = c(50, 55, 60, 65, 70)                                     # Age groups  
# Male = c(15.4, 24.3, 37.0, 54.6, 71.1)                          # Death rates for males  
# Female = c(8.4, 13.6, 19.3, 35.1, 50.0)                         # Death rates for females  
# Deathrate = matrix(c(Male,Female), nrow=length(Age), ncol=2, dimnames=list(Age, c("Male","Female")))         
# # Create a matrix Deathrate with the data  
# Deathrate2 = t(Deathrate)                                       # Transpose the Deathrate matrix  
  
# # SIMPLE BARPLOT  
  
#                                        # Open a new window for the plot  
  
# barplot(Deathrate2,                             # Data (bar heights) to plot  
#         beside=TRUE,                            # Plot the bars beside one another; default is to plot stacked bars  
#         space=c(0.2,0.8),                       # Amount of space between i) bars within a group, ii) bars between groups  
#         names.arg=c("65-69", "60-64", "55-59", "50-54", "70-74"),    #Names for the bars  
#         col=c("blue", "red"),                   # Color of the bars  
#         border="black",                         # Color of the bar borders  
#         main=c("Death rates in Virginia"),      # Main title for the plot  
#         xlab="Age group",                       # X-axis label  
#         ylab="Death rate",                      # Y-axis label  
#         font.lab=2)                             # Font to use for the axis labels: 1=plain text, 2=bold, 3=italic, 4=bold italic  
  
# legend("topleft",                               # Add a legend to the plot  
#         legend=c("Male", "Female"),             # Text for the legend  
#         fill=c("blue", "red")) 
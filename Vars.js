.pragma library

var numpadValue = 0
var numpadMaximum = 1000
var numpadMinimum = 0
var keypadDecimalPlaces = 0
var recipeSeclection = 0
var keypadCurrentDecimalPlace=0
var statementText = "statement text"
var statementVisible = false
var inputtedText = ""
var keyboardVisible = false
var numpadVisible = false
var numpadAddress=-1
var numpadReset = false
var numpadDoubleWord=false
var currentScreen=0
var currentScaler=1.0
var recipesRegisterMode=false
var recipeStatusText="Recipe Register mode"
var currentRecipeNum=0
var currentRecipeText=""
var recipesListOffset=0
var editMode=false
var currentRegisterType=0
var currentRegisterAddress=""
var editPadVisible=false
var currentRegisterID=""
var currentRegisterString=""
var currentModuleSelected=1
var securityLevel=10
var loggedUser=""
var loggedUserFirstName=""
var loggedUserLastName=""
var featureConfiguration=false
var positionConfiguration=false
var alarmVisibility=false
var registerVisibility=false
var registerObjects=[]



var inspectionRegions=[]
var numOfInspectionRegions=0
var currentRegion=0
var horizontalLines=[]
var verticalLines=[]


function rectangle(posx,posy,w,h,refSum){
    this.posx=posx
    this.posy=posy
    this.w=w
    this.h=h
    this.refSum=refSum
}

var globalTimer

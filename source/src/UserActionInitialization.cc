//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// UserActionInitialization.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "UserActionInitialization.hh"
#include "PrimaryGenerator.hh"
#include "EventAction.hh"
#include "RunAction.hh"

//------------------------------------------------------------------------------
  UserActionInitialization::UserActionInitialization()
  : G4VUserActionInitialization()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
  UserActionInitialization::~UserActionInitialization()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
  void UserActionInitialization::Build() const
//------------------------------------------------------------------------------
{
    EventAction* eventaction = new EventAction();
    SetUserAction( new PrimaryGenerator() );
    SetUserAction(eventaction);
//    SetUserAction( new SteppingAction(eventaction) );　　不要
    SetUserAction(new RunAction);
}

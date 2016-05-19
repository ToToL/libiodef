# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.8
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

package Iodef;
use base qw(Exporter);
use base qw(DynaLoader);
package Iodefc;
bootstrap Iodef;
package Iodef;
@EXPORT = qw();

# ---------- BASE METHODS -------------

package Iodef;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package Iodef;


############# Class : Iodef::PreludeError ##############

package Iodef::PreludeError;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Iodef );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Iodefc::delete_PreludeError($self);
        delete $OWNER{$self};
    }
}

sub new {
    my $pkg = shift;
    my $self = Iodefc::new_PreludeError(@_);
    bless $self, $pkg if defined($self);
}

*getCode = *Iodefc::PreludeError_getCode;
*what = *Iodefc::PreludeError_what;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : Iodef::IODEFCriterion ##############

package Iodef::IODEFCriterion;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Iodef );
%OWNER = ();
%ITERATORS = ();
*OPERATOR_NOT = *Iodefc::IODEFCriterion_OPERATOR_NOT;
*OPERATOR_NOCASE = *Iodefc::IODEFCriterion_OPERATOR_NOCASE;
*OPERATOR_EQUAL = *Iodefc::IODEFCriterion_OPERATOR_EQUAL;
*OPERATOR_EQUAL_NOCASE = *Iodefc::IODEFCriterion_OPERATOR_EQUAL_NOCASE;
*OPERATOR_NOT_EQUAL = *Iodefc::IODEFCriterion_OPERATOR_NOT_EQUAL;
*OPERATOR_NOT_EQUAL_NOCASE = *Iodefc::IODEFCriterion_OPERATOR_NOT_EQUAL_NOCASE;
*OPERATOR_LESSER = *Iodefc::IODEFCriterion_OPERATOR_LESSER;
*OPERATOR_LESSER_OR_EQUAL = *Iodefc::IODEFCriterion_OPERATOR_LESSER_OR_EQUAL;
*OPERATOR_GREATER = *Iodefc::IODEFCriterion_OPERATOR_GREATER;
*OPERATOR_GREATER_OR_EQUAL = *Iodefc::IODEFCriterion_OPERATOR_GREATER_OR_EQUAL;
*OPERATOR_SUBSTR = *Iodefc::IODEFCriterion_OPERATOR_SUBSTR;
*OPERATOR_SUBSTR_NOCASE = *Iodefc::IODEFCriterion_OPERATOR_SUBSTR_NOCASE;
*OPERATOR_NOT_SUBSTR = *Iodefc::IODEFCriterion_OPERATOR_NOT_SUBSTR;
*OPERATOR_NOT_SUBSTR_NOCASE = *Iodefc::IODEFCriterion_OPERATOR_NOT_SUBSTR_NOCASE;
*OPERATOR_REGEX = *Iodefc::IODEFCriterion_OPERATOR_REGEX;
*OPERATOR_REGEX_NOCASE = *Iodefc::IODEFCriterion_OPERATOR_REGEX_NOCASE;
*OPERATOR_NOT_REGEX = *Iodefc::IODEFCriterion_OPERATOR_NOT_REGEX;
*OPERATOR_NOT_REGEX_NOCASE = *Iodefc::IODEFCriterion_OPERATOR_NOT_REGEX_NOCASE;
*OPERATOR_NULL = *Iodefc::IODEFCriterion_OPERATOR_NULL;
*OPERATOR_NOT_NULL = *Iodefc::IODEFCriterion_OPERATOR_NOT_NULL;
sub new {
    my $pkg = shift;
    my $self = Iodefc::new_IODEFCriterion(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Iodefc::delete_IODEFCriterion($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : Iodef::IODEFCriteria ##############

package Iodef::IODEFCriteria;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Iodef );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Iodefc::delete_IODEFCriteria($self);
        delete $OWNER{$self};
    }
}

sub new {
    my $pkg = shift;
    my $self = Iodefc::new_IODEFCriteria(@_);
    bless $self, $pkg if defined($self);
}

*match = *Iodefc::IODEFCriteria_match;
*clone = *Iodefc::IODEFCriteria_clone;
*andCriteria = *Iodefc::IODEFCriteria_andCriteria;
*orCriteria = *Iodefc::IODEFCriteria_orCriteria;
*toString = *Iodefc::IODEFCriteria_toString;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : Iodef::IODEFValue ##############

package Iodef::IODEFValue;
use overload
    "<=" => sub { $_[0]->__le__($_[1])},
    ">=" => sub { $_[0]->__ge__($_[1])},
    "<" => sub { $_[0]->__lt__($_[1])},
    "!=" => sub { $_[0]->__ne__($_[1])},
    "==" => sub { $_[0]->__eq__($_[1])},
    ">" => sub { $_[0]->__gt__($_[1])},
    "=" => sub { my $class = ref($_[0]); $class->new($_[0]) },
    "fallback" => 1;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Iodef );
%OWNER = ();
%ITERATORS = ();
*TYPE_UNKNOWN = *Iodefc::IODEFValue_TYPE_UNKNOWN;
*TYPE_INT8 = *Iodefc::IODEFValue_TYPE_INT8;
*TYPE_UINT8 = *Iodefc::IODEFValue_TYPE_UINT8;
*TYPE_INT16 = *Iodefc::IODEFValue_TYPE_INT16;
*TYPE_UINT16 = *Iodefc::IODEFValue_TYPE_UINT16;
*TYPE_INT32 = *Iodefc::IODEFValue_TYPE_INT32;
*TYPE_UINT32 = *Iodefc::IODEFValue_TYPE_UINT32;
*TYPE_INT64 = *Iodefc::IODEFValue_TYPE_INT64;
*TYPE_UINT64 = *Iodefc::IODEFValue_TYPE_UINT64;
*TYPE_FLOAT = *Iodefc::IODEFValue_TYPE_FLOAT;
*TYPE_DOUBLE = *Iodefc::IODEFValue_TYPE_DOUBLE;
*TYPE_STRING = *Iodefc::IODEFValue_TYPE_STRING;
*TYPE_TIME = *Iodefc::IODEFValue_TYPE_TIME;
*TYPE_DATA = *Iodefc::IODEFValue_TYPE_DATA;
*TYPE_ENUM = *Iodefc::IODEFValue_TYPE_ENUM;
*TYPE_LIST = *Iodefc::IODEFValue_TYPE_LIST;
*TYPE_CLASS = *Iodefc::IODEFValue_TYPE_CLASS;
*getType = *Iodefc::IODEFValue_getType;
*isNull = *Iodefc::IODEFValue_isNull;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Iodefc::delete_IODEFValue($self);
        delete $OWNER{$self};
    }
}

sub new {
    my $pkg = shift;
    my $self = Iodefc::new_IODEFValue(@_);
    bless $self, $pkg if defined($self);
}

*match = *Iodefc::IODEFValue_match;
*clone = *Iodefc::IODEFValue_clone;
*toString = *Iodefc::IODEFValue_toString;
*__le__ = *Iodefc::IODEFValue___le__;
*__ge__ = *Iodefc::IODEFValue___ge__;
*__lt__ = *Iodefc::IODEFValue___lt__;
*__gt__ = *Iodefc::IODEFValue___gt__;
*__eq__ = *Iodefc::IODEFValue___eq__;
*__ne__ = *Iodefc::IODEFValue___ne__;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : Iodef::IODEFPath ##############

package Iodef::IODEFPath;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Iodef );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = Iodefc::new_IODEFPath(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Iodefc::delete_IODEFPath($self);
        delete $OWNER{$self};
    }
}

*set = *Iodefc::IODEFPath_set;
*getClass = *Iodefc::IODEFPath_getClass;
*getValueType = *Iodefc::IODEFPath_getValueType;
*setIndex = *Iodefc::IODEFPath_setIndex;
*undefineIndex = *Iodefc::IODEFPath_undefineIndex;
*getIndex = *Iodefc::IODEFPath_getIndex;
*makeChild = *Iodefc::IODEFPath_makeChild;
*makeParent = *Iodefc::IODEFPath_makeParent;
*compare = *Iodefc::IODEFPath_compare;
*clone = *Iodefc::IODEFPath_clone;
*checkOperator = *Iodefc::IODEFPath_checkOperator;
*getApplicableOperators = *Iodefc::IODEFPath_getApplicableOperators;
*getName = *Iodefc::IODEFPath_getName;
*isAmbiguous = *Iodefc::IODEFPath_isAmbiguous;
*hasLists = *Iodefc::IODEFPath_hasLists;
*isList = *Iodefc::IODEFPath_isList;
*getDepth = *Iodefc::IODEFPath_getDepth;
*get = *Iodefc::IODEFPath_get;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : Iodef::IODEFTime ##############

package Iodef::IODEFTime;
use overload
    "<=" => sub { $_[0]->__le__($_[1])},
    "!=" => sub { $_[0]->__ne__($_[1])},
    ">=" => sub { $_[0]->__ge__($_[1])},
    "<" => sub { $_[0]->__lt__($_[1])},
    "==" => sub { $_[0]->__eq__($_[1])},
    ">" => sub { $_[0]->__gt__($_[1])},
    "=" => sub { my $class = ref($_[0]); $class->new($_[0]) },
    "fallback" => 1;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Iodef );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = Iodefc::new_IODEFTime(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Iodefc::delete_IODEFTime($self);
        delete $OWNER{$self};
    }
}

*set = *Iodefc::IODEFTime_set;
*setSec = *Iodefc::IODEFTime_setSec;
*setUSec = *Iodefc::IODEFTime_setUSec;
*setGmtOffset = *Iodefc::IODEFTime_setGmtOffset;
*getSec = *Iodefc::IODEFTime_getSec;
*getUSec = *Iodefc::IODEFTime_getUSec;
*getGmtOffset = *Iodefc::IODEFTime_getGmtOffset;
*getTime = *Iodefc::IODEFTime_getTime;
*clone = *Iodefc::IODEFTime_clone;
*toString = *Iodefc::IODEFTime_toString;
*__ne__ = *Iodefc::IODEFTime___ne__;
*__ge__ = *Iodefc::IODEFTime___ge__;
*__le__ = *Iodefc::IODEFTime___le__;
*__eq__ = *Iodefc::IODEFTime___eq__;
*__gt__ = *Iodefc::IODEFTime___gt__;
*__lt__ = *Iodefc::IODEFTime___lt__;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : Iodef::IODEFClass ##############

package Iodef::IODEFClass;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Iodef );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = Iodefc::new_IODEFClass(@_);
    bless $self, $pkg if defined($self);
}

*getDepth = *Iodefc::IODEFClass_getDepth;
*get = *Iodefc::IODEFClass_get;
*getChildCount = *Iodefc::IODEFClass_getChildCount;
*isList = *Iodefc::IODEFClass_isList;
*isKeyedList = *Iodefc::IODEFClass_isKeyedList;
*getName = *Iodefc::IODEFClass_getName;
*toString = *Iodefc::IODEFClass_toString;
*getValueType = *Iodefc::IODEFClass_getValueType;
*getPath = *Iodefc::IODEFClass_getPath;
*getEnumValues = *Iodefc::IODEFClass_getEnumValues;
*getApplicableOperator = *Iodefc::IODEFClass_getApplicableOperator;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Iodefc::delete_IODEFClass($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : Iodef::IODEF ##############

package Iodef::IODEF;
use overload
    "==" => sub { $_[0]->__eq__($_[1])},
    "=" => sub { my $class = ref($_[0]); $class->new($_[0]) },
    "fallback" => 1;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Iodef );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Iodefc::delete_IODEF($self);
        delete $OWNER{$self};
    }
}

sub new {
    my $pkg = shift;
    my $self = Iodefc::new_IODEF(@_);
    bless $self, $pkg if defined($self);
}

*set = *Iodefc::IODEF_set;
*clone = *Iodefc::IODEF_clone;
*getId = *Iodefc::IODEF_getId;
*toString = *Iodefc::IODEF_toString;
*toJSON = *Iodefc::IODEF_toJSON;
*toBinary = *Iodefc::IODEF_toBinary;
*__eq__ = *Iodefc::IODEF___eq__;
*get = *Iodefc::IODEF_get;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


# ------- VARIABLE STUBS --------

package Iodef;

1;

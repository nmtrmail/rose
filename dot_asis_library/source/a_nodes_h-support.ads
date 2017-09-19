with Asis.Set_Get;
with Unchecked_Conversion;

package a_nodes_h.Support is
   -- Records written in good Ada style should already have default values for
   -- their components. a_nodes_h.ads is generated from C, so this package
   -- supplies constant records for safe initialization.

   package ICE renames Interfaces.C.Extensions;
   package ICS renames Interfaces.C.Strings;

   Invalid_bool      : constant ICE.bool := 0;
   Invalid_chars_ptr : constant ICS.chars_ptr := ICS.Null_Ptr;
   Invalid_Node_ID   : constant Node_ID := -1;

   -- Order below is same as in a_nodes.h:

   Default_Context_Struct : constant Context_Struct :=
     (name        => Invalid_chars_ptr,
      parameters  => Invalid_chars_ptr,
      debug_image => Invalid_chars_ptr);

   Empty_Unit_List : constant Unit_List :=
     (length => 0,
      IDs => null);

   Default_Unit_Struct : constant Unit_Struct :=
     (ID                                => Invalid_Node_ID,
      Unit_Kind                         => Not_A_Unit,
      Unit_Class                        => Not_A_Class,
      Unit_Origin                       => Not_An_Origin,
      Corresponding_Children            => Empty_Unit_List,
      Corresponding_Parent_Declaration  => Invalid_Node_ID,
      Corresponding_Declaration         => Invalid_Node_ID,
      Corresponding_Body                => Invalid_Node_ID,
      Unit_Full_Name                    => Invalid_chars_ptr,
      Unique_Name                       => Invalid_chars_ptr,
      Exists                            => Invalid_bool,
      Can_Be_Main_Program               => Invalid_bool,
      Is_Body_Required                  => Invalid_bool,
      Text_Name                         => Invalid_chars_ptr,
      Text_Form                         => Invalid_chars_ptr,
      Object_Name                       => Invalid_chars_ptr,
      Object_Form                       => Invalid_chars_ptr,
      Compilation_Command_Line_Options  => Invalid_chars_ptr,
      Subunits                          => Empty_Unit_List,
      Corresponding_Subunit_Parent_Body => Invalid_Node_ID,
      Debug_Image                       => Invalid_chars_ptr);

   -- Element union component default structs go here

   Empty_Element_List    : constant Element_List :=
     (length => 0,
      IDs => null);
   Empty_Name_List       : constant Name_List :=
     Name_List (Empty_Element_List);

   Default_Pragma_Struct : constant Pragma_Struct :=
     (Pragma_Kind => Not_A_Pragma);

   Default_Defining_Name_Struct : constant Defining_Name_Struct :=
     (Defining_Name_Kind                 => Not_A_Defining_Name,
      Defining_Name_Image                => Invalid_chars_ptr,
      Position_Number_Image              => Invalid_chars_ptr,
      Representation_Value_Image         => Invalid_chars_ptr,
      Defining_Prefix                    => Invalid_Node_ID,
      Defining_Selector                  => Invalid_Node_ID,
      Corresponding_Constant_Declaration => Invalid_Node_ID,
      Operator_Kind                      => Not_An_Operator,
      Corresponding_Generic_Element      => Invalid_Node_ID);

   Default_Declaration_Struct : constant Declaration_Struct :=
     (Declaration_Kind                     => Not_A_Declaration,
      Declaration_Origin                   => Not_A_Declaration_Origin,
      Mode_Kind                            => Not_A_Mode,
      Default_Kind                         => Not_A_Default,
      Trait_Kind                           => Not_A_Trait,
      Names                                => Empty_Element_List,
      Discriminant_Part                    => Invalid_Node_ID,
      Type_Declaration_View                => Invalid_Node_ID,
      Object_Declaration_View              => Invalid_Node_ID,
      Aspect_Specifications                => Empty_Element_List,
      Initialization_Expression            => Invalid_Node_ID,
      Corresponding_Type_Declaration       => Invalid_Node_ID,
      Corresponding_Type_Completion        => Invalid_Node_ID,
      Corresponding_Type_Partial_View      => Invalid_Node_ID,
      Corresponding_First_Subtype          => Invalid_Node_ID,
      Corresponding_Last_Constraint        => Invalid_Node_ID,
      Corresponding_Last_Subtype           => Invalid_Node_ID,
      Corresponding_Representation_Clauses => Empty_Element_List,
      Specification_Subtype_Definition     => Invalid_Node_ID,
      Iteration_Scheme_Name                => Invalid_Node_ID,
      Subtype_Indication                   => Invalid_Node_ID,
      Parameter_Profile                    => Empty_Element_List,
      Result_Profile                       => Invalid_Node_ID,
      Result_Expression                    => Invalid_Node_ID,
      Is_Overriding_Declaration            => Invalid_bool,
      Is_Not_Overriding_Declaration        => Invalid_bool,
      Body_Declarative_Items               => Empty_Element_List,
      Body_Statements                      => Empty_Element_List,
      Body_Exception_Handlers              => Empty_Element_List,
      Body_Block_Statement                 => Invalid_Node_ID,
      Is_Name_Repeated                     => Invalid_bool,
      Corresponding_Declaration            => Invalid_Node_ID,
      Corresponding_Body                   => Invalid_Node_ID,
      Corresponding_Subprogram_Derivation  => Invalid_Node_ID,
      Corresponding_Type                   => Invalid_Node_ID,
      Corresponding_Equality_Operator      => Invalid_Node_ID,
      Visible_Part_Declarative_Items       => Empty_Element_List,
      Is_Private_Present                   => Invalid_bool,
      Private_Part_Declarative_Items       => Empty_Element_List,
      Declaration_Interface_List           => Empty_Element_List,
      Renamed_Entity                       => Invalid_Node_ID,
      Corresponding_Base_Entity            => Invalid_Node_ID,
      Protected_Operation_Items            => Empty_Element_List,
      Entry_Family_Definition              => Invalid_Node_ID,
      Entry_Index_Specification            => Invalid_Node_ID,
      Entry_Barrier                        => Invalid_Node_ID,
      Corresponding_Subunit                => Invalid_Node_ID,
      Is_Subunit                           => Invalid_bool,
      Corresponding_Body_Stub              => Invalid_Node_ID,
      Generic_Formal_Part                  => Empty_Element_List,
      Generic_Unit_Name                    => Invalid_Node_ID,
      Generic_Actual_Part                  => Empty_Element_List,
      Formal_Subprogram_Default            => Invalid_Node_ID,
      Is_Dispatching_Operation             => Invalid_bool);

   Default_Definition_Struct : constant Definition_Struct :=
     (Definition_Kind                 => Not_A_Definition,
      Trait_Kind                      => Not_A_Trait,
      Type_Kind                       => Not_A_Type_Definition,
      Parent_Subtype_Indication       => Invalid_Node_ID,
      Record_Definition               => Invalid_Node_ID,
      Implicit_Inherited_Declarations => Empty_Element_List,
      Implicit_Inherited_Subprograms  => Empty_Element_List,
      Corresponding_Parent_Subtype    => Invalid_Node_ID,
      Corresponding_Root_Type         => Invalid_Node_ID,
      Corresponding_Type_Structure    => Invalid_Node_ID,
      Constraint_Kind                 => Not_A_Constraint,
      Lower_Bound                     => Invalid_Node_ID,
      Upper_Bound                     => Invalid_Node_ID,
      Subtype_Mark                    => Invalid_Node_ID,
      Subtype_Constraint              => Invalid_Node_ID,
      Component_Subtype_Indication    => Invalid_Node_ID,
      Component_Definition_View       => Invalid_Node_ID,
      Record_Components               => Empty_Element_List,
      Implicit_Components             => Empty_Element_List,
      Visible_Part_Items              => Empty_Element_List,
      Private_Part_Items              => Empty_Element_List,
      Is_Private_Present              => Invalid_bool
     );

   Default_Expression_Struct : constant Expression_Struct :=
     (Expression_Kind                          => Not_An_Expression,
      Corresponding_Expression_Type            => Invalid_Node_ID,
      Value_Image                              => Invalid_chars_ptr,
      Name_Image                               => Invalid_chars_ptr,
      Corresponding_Name_Definition            => Invalid_Node_ID,
      Corresponding_Name_Definition_List       => Empty_Element_List,
      Corresponding_Name_Declaration           => Invalid_Node_ID,
      Operator_Kind                            => Not_An_Operator,
      Prefix                                   => Invalid_Node_ID,
      Corresponding_Called_Function            => Invalid_Node_ID,
      Is_Prefix_Call                           => Invalid_bool,
      Function_Call_Parameters                 => Empty_Element_List,
      Index_Expressions                        => Empty_Element_List,
      Is_Generalized_Indexing                  => Invalid_bool,
      Slice_Range                              => Invalid_Node_ID,
      Selector                                 => Invalid_Node_ID,
      Atribute_Kind                            => Not_An_Attribute,
      Attribute_Designator_Identifier          => Invalid_Node_ID,
      Attribute_Designator_Expressions         => Empty_Element_List,
      Record_Component_Associations            => Empty_Element_List,
      Extension_Aggregate_Expression           => Invalid_Node_ID,
      Array_Component_Associations             => Empty_Element_List,
      Short_Circuit_Operation_Left_Expression  => Invalid_Node_ID,
      Short_Circuit_Operation_Right_Expression => Invalid_Node_ID,
      Membership_Test_Expression               => Invalid_Node_ID,
      Membership_Test_Choices                  => Empty_Element_List,
      Expression_Parenthesized                 => Invalid_Node_ID,
      Converted_Or_Qualified_Subtype_Mark      => Invalid_Node_ID,
      Converted_Or_Qualified_Expression        => Invalid_Node_ID,
      Predicate                                => Invalid_Node_ID,
      Subpool_Name                             => Invalid_Node_ID,
      Allocator_Subtype_Indication             => Invalid_Node_ID,
      Allocator_Qualified_Expression           => Invalid_Node_ID,
      Expression_Paths                         => Empty_Element_List,
      Iterator_Specification                   => Invalid_Node_ID,
      Corresponding_Generic_Element            => Invalid_Node_ID);

   Default_Association_Struct : constant Association_Struct :=
     (Association_Kind            => Not_An_Association,
      Array_Component_Choices     => Empty_Element_List,
      Record_Component_Choices    => Empty_Element_List,
      Component_Expression        => Invalid_Node_ID,
      Formal_Parameter            => Invalid_Node_ID,
      Actual_Parameter            => Invalid_Node_ID,
      Discriminant_Selector_Names => Empty_Element_List,
      Discriminant_Expression     => Invalid_Node_ID,
      Is_Normalized               => Invalid_bool,
      Is_Defaulted_Association    => Invalid_bool);

   Default_Statement_Struct : constant Statement_Struct :=
     (Statement_Kind                      => Not_A_Statement,
      Label_Names                         => Empty_Element_List,
      Assignment_Variable_Name            => Invalid_Node_ID,
      Assignment_Expression               => Invalid_Node_ID,
      Statement_Paths                     => Empty_Element_List,
      Case_Expression                     => Invalid_Node_ID,
      Statement_Identifier                => Invalid_Node_ID,
      Is_Name_Repeated                    => Invalid_bool,
      While_Condition                     => Invalid_Node_ID,
      For_Loop_Parameter_Specification    => Invalid_Node_ID,
      Loop_Statements                     => Empty_Element_List,
      Is_Declare_Block                    => Invalid_bool,
      Block_Declarative_Items             => Empty_Element_List,
      Block_Statements                    => Empty_Element_List,
      Block_Exception_Handlers            => Empty_Element_List,
      Exit_Loop_Name                      => Invalid_Node_ID,
      Exit_Condition                      => Invalid_Node_ID,
      Corresponding_Loop_Exited           => Invalid_Node_ID,
      Goto_Label                          => Invalid_Node_ID,
      Corresponding_Destination_Statement => Invalid_Node_ID,
      Called_Name                         => Invalid_Node_ID,
      Corresponding_Called_Entity         => Invalid_Node_ID,
      Call_Statement_Parameters           => Empty_Element_List,
      Return_Expression                   => Invalid_Node_ID,
      Return_Object_Declaration           => Invalid_Node_ID,
      Extended_Return_Statements          => Empty_Element_List,
      Extended_Return_Exception_Handlers  => Empty_Element_List,
      Accept_Entry_Index                  => Invalid_Node_ID,
      Accept_Entry_Direct_Name            => Invalid_Node_ID,
      Accept_Parameters                   => Empty_Element_List,
      Accept_Body_Statements              => Empty_Element_List,
      Accept_Body_Exception_Handlers      => Empty_Element_List,
      Corresponding_Entry                 => Invalid_Node_ID,
      Requeue_Entry_Name                  => Invalid_Node_ID,
      Delay_Expression                    => Invalid_Node_ID,
      Aborted_Tasks                       => Empty_Element_List,
      Raised_Exception                    => Invalid_Node_ID,
      Associated_Message                  => Invalid_Node_ID,
      Qualified_Expression                => Invalid_Node_ID);

   Default_Path_Struct : constant Path_Struct :=
     (Path_Kind                     => Not_A_Path,
      Sequence_Of_Statements        => Empty_Element_List,
      Condition_Expression          => Invalid_Node_ID,
      Case_Path_Alternative_Choices => Empty_Element_List,
      Guard                         => Invalid_Node_ID);

   Default_Clause_Struct : constant Clause_Struct :=
     (Clause_Kind  => Not_A_Clause,
      Trait_Kind   => Not_A_Trait,
      Clause_Names => Empty_Name_List);

   Default_Exception_Handler_Struct : constant Exception_Handler_Struct :=
     (Choice_Parameter_Specification => Invalid_Node_ID,
      Exception_Choices              => Empty_Element_List,
      Handler_Statements             => Empty_Element_List);

   Default_Element_Union : constant Element_Union :=
     (Discr        => 0,
      Dummy_Member => 0);

   Default_Source_Location_Struct : constant Source_Location_Struct :=
     (Unit_Name    => Invalid_chars_ptr,
      First_Line   => -1,
      First_Column => -1,
      Last_Line    => -1,
      Last_Column  => -1);

   Default_Element_Struct : constant Element_Struct :=
     (ID                   => Invalid_Node_ID,
      Element_Kind         => Not_An_Element,
      Enclosing_Element_Id => Invalid_Node_ID,
      Enclosing_Kind       => Not_Enclosing,
      Source_Location      => Default_Source_Location_Struct,
      The_Union            => Default_Element_Union);

   Default_Node_Union : constant Node_Union :=
     (Discr        => 0,
      Dummy_member => 0);

   Default_Node_Struct : constant Node_Struct :=
     (Node_kind => Not_A_Node,
      The_Union => Default_Node_Union);

   Default_List_Node_Struct : constant List_Node_Struct :=
     (Node       => Default_Node_Struct,
      Next       => null,
      Next_count => 0);


   -- Order below is alphabetical:
   function To_Association_Kinds is new Unchecked_Conversion
     (Source => Asis.Association_Kinds,
      Target => a_nodes_h.Association_Kinds);

   function To_Attribute_Kinds is new Unchecked_Conversion
     (Source => Asis.Attribute_Kinds,
      Target => a_nodes_h.Attribute_Kinds);

   function To_Clause_Kinds is new Unchecked_Conversion
     (Source => Asis.Clause_Kinds,
      Target => a_nodes_h.Clause_Kinds);

   function To_Constraint_Kinds is new Unchecked_Conversion
     (Source => Asis.Constraint_Kinds,
      Target => a_nodes_h.Constraint_Kinds);

   function To_Declaration_Kinds is new Unchecked_Conversion
     (Source => Asis.Declaration_Kinds,
      Target => a_nodes_h.Declaration_Kinds);

   function To_Declaration_Origins is new Unchecked_Conversion
     (Source => Asis.Declaration_Origins,
      Target => a_nodes_h.Declaration_Origins);

   function To_Defining_Name_Kinds is new Unchecked_Conversion
     (Source => Asis.Defining_Name_Kinds,
      Target => a_nodes_h.Defining_Name_Kinds);

   function To_Definition_Kinds is new Unchecked_Conversion
     (Source => Asis.Definition_Kinds,
      Target => a_nodes_h.Definition_Kinds);

   function To_Element_Kinds is new Unchecked_Conversion
     (Source => Asis.Element_Kinds,
      Target => a_nodes_h.Element_Kinds);

   function To_Expression_Kinds is new Unchecked_Conversion
     (Source => Asis.Expression_Kinds,
      Target => a_nodes_h.Expression_Kinds);

   function To_Mode_Kinds is new Unchecked_Conversion
     (Source => Asis.Mode_Kinds,
      Target => a_nodes_h.Mode_Kinds);

   function To_Operator_Kinds is new Unchecked_Conversion
     (Source => Asis.Operator_Kinds,
      Target => a_nodes_h.Operator_Kinds);

   function To_Path_Kinds is new Unchecked_Conversion
     (Source => Asis.Path_Kinds,
      Target => a_nodes_h.Path_Kinds);

   function To_Pragma_Kinds is new Unchecked_Conversion
     (Source => Asis.Pragma_Kinds,
      Target => a_nodes_h.Pragma_Kinds);

   function To_Statement_Kinds is new Unchecked_Conversion
     (Source => Asis.Statement_Kinds,
      Target => a_nodes_h.Statement_Kinds);

   function To_Subprogram_Default_Kinds is new Unchecked_Conversion
     (Source => Asis.Subprogram_Default_Kinds,
      Target => a_nodes_h.Subprogram_Default_Kinds);

   function To_Trait_Kinds is new Unchecked_Conversion
     (Source => Asis.Trait_Kinds,
      Target => a_nodes_h.Trait_Kinds);

   function To_Type_Kinds is new Unchecked_Conversion
     (Source => Asis.Type_Kinds,
      Target => a_nodes_h.Type_Kinds);

   function To_Unit_Classes is new Unchecked_Conversion
     (Source => Asis.Unit_Classes,
      Target => a_nodes_h.Unit_Classes);

   function To_Unit_Kinds is new Unchecked_Conversion
     (Source => Asis.Unit_Kinds,
      Target => a_nodes_h.Unit_Kinds);

   function To_Unit_Origins is new Unchecked_Conversion
     (Source => Asis.Unit_Origins,
      Target => a_nodes_h.Unit_Origins);

   -- End alphabetical order

   function To_Element_ID
     (Item : in Asis.Element)
      return a_nodes_h.Node_ID
   is
     (a_nodes_h.Node_ID (Asis.Set_Get.Node_Value (Item)));

   -- Not in a_nodes.h:

   function To_bool
     (Item : in Boolean)
      return ICE.bool
   is
     (if Item then 1 else 0);

   type Unit_ID_Array is array (Positive range <>) of aliased Unit_ID;
   -- Not called _Ptr so we don't forget a pointer to this is not the same as a
   -- pointer to a C array.  We just need this to create the array on the hea:
   type Unit_ID_Array_Access is access Unit_ID_Array;

   function To_Unit_ID_Ptr
     (Item : not null access Unit_ID_Array)
      return Unit_ID_Ptr is
     (if Item.all'Length = 0 then
         null
      else
         Item.all (Item.all'First)'Unchecked_Access);

   type Element_ID_Array is array (Positive range <>) of aliased Element_ID;
   -- Not called _Ptr so we don't forget a pointer to this is not the same as a
   -- pointer to a C array.  We just need this to create the array on the hea:
   type Element_ID_Array_Access is access Element_ID_Array;

   function To_Element_ID_Ptr
     (Item : not null access Element_ID_Array)
      return Element_ID_Ptr is
     (if Item.all'Length = 0 then
         null
      else
         Item.all (Item.all'First)'Unchecked_Access);

end a_nodes_h.Support;

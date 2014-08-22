<?xml version="1.0" encoding="UTF-8"?>
<tileset name="objects" tilewidth="16" tileheight="16">
 <image source="objects.png" width="512" height="512"/>
 <tile id="33">
  <properties>
   <property name="CreateEntity" value="mushroomSmall"/>
  </properties>
 </tile>
 <tile id="35">
  <properties>
   <property name="CreateEntity" value="slime"/>
   <property name="int:jumpVeloc" value="280"/>
  </properties>
 </tile>
 <tile id="97">
  <properties>
   <property name="CreateEntity" value="block"/>
  </properties>
 </tile>
 <tile id="135">
  <properties>
   <property name="CreateEntity" value="rock_mouth"/>
   <property name="int:offsetY" value="16"/>
  </properties>
 </tile>
 <tile id="193">
  <properties>
   <property name="CreateEntity" value="placeable"/>
   <property name="SetAnim" value="flower0"/>
  </properties>
 </tile>
 <tile id="257">
  <properties>
   <property name="CreateEntity" value="collectible"/>
   <property name="SetAnim" value="Coin_Idle"/>
   <property name="SetAnimDying" value="Coin_Dying"/>
  </properties>
 </tile>
 <tile id="355">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="3"/>
   <property name="int:itemType" value="0"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="356">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="1"/>
   <property name="int:itemType" value="0"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="357">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="2"/>
   <property name="int:itemType" value="0"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="358">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="4"/>
   <property name="int:itemType" value="0"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="385">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="4"/>
   <property name="int:itemType" value="0"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="387">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="2"/>
   <property name="int:itemType" value="1"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="388">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="1"/>
   <property name="int:itemType" value="1"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="389">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="3"/>
   <property name="int:itemType" value="1"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="390">
  <properties>
   <property name="CreateEntity" value="chest"/>
   <property name="int:itemSubtype" value="4"/>
   <property name="int:itemType" value="0"/>
   <property name="int:offsetX" value="16"/>
   <property name="int:offsetY" value="0"/>
  </properties>
 </tile>
 <tile id="640">
  <properties>
   <property name="CreateEntity" value="placeable"/>
   <property name="SetAnim" value="Darkness1"/>
   <property name="SetSprite" value="sprites\deco01.sprite"/>
  </properties>
 </tile>
</tileset>

<h1>H5N1 Games Programming 2 Final Submission</h1>

<h2>Submission</h2>

<h3>The Members</h3>
<p>Brian Hannah</p>
<p>Anthony Denovan</p>
<p>Marc Clelland</p>
<p>Andrew Cassidy</p>

<h3>Environment Variables</h3>

<P></P>

<h3>Include and Linker settings</h3>

<p>In Visual Studio you should setup up the following project properties</p>

<table border="1">
<tr>
  <td>C/C++ - Include Directories</td>
  <td>This will need to be the path to the directory where Fmod is stored\inc</td>
</tr>
<tr>
	<td>Linker - Additional Library Directories</td>
	<td>This will need to be the path to the directory where Fmod is stored\lib</td>
</tr>
<tr>
	<td>Linker - Input - Additional Dependencies</td>
	<td>fmodL_vc.lib</td>
<tr>
</table>
 
<p>You should also copy the following dll from the FMOD install directory, it should be located in the api folder, to the visual studio project directory</p>
<b>fmodL.dll</b> 



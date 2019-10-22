var mainTitleColor = '"#08519c"';
var sectionHeaderColor = '"#3182BD"';
var navTableBGColor = '"#BDD7E7"';

function topOfPageTable(pathToRoot)
{
	document.write('<center><h1><font color=' + mainTitleColor + '>Shader-Based OpenGL</font></h1></center>');

}

// return value is what you write, if anything, to close the tag
function openFileTag(pathToRoot, currentRef, pathFromRootToAnchorRef, anchorRef)
{
	if ((currentRef == anchorRef) || ((currentRef == "") && (anchorRef == "index.html")))
	{
		document.write('<b>');
		return '</b>';
	}
	document.write('<a href="' + pathToRoot + pathFromRootToAnchorRef + anchorRef + '">');
	return '</a>';
}

function navigationTable(pathToRoot)
{
	fName = window.location.href.substring(window.location.href.lastIndexOf("/")+1,
		window.location.href.length);
	document.write('<ul style="padding-left:15px">');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "", "OpenGL.html");
	document.write('Home' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "Background.html");
	document.write('Background' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "Versions.html");
	document.write('Versions' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "Architecture.html");
	document.write('Architecture' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "CourseGoals.html");
	document.write('Course Goals' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "HelloOpenGL.html");
	document.write('Hello, OpenGL' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "DataTypes.html");
	document.write('Data Types' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "DrawModes.html");
	document.write('Draw Modes' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "GettingStarted.html");
	document.write('Getting Started' + closeTag + '</li>');

	document.write('<li style="margin-bottom:1em">');
	closeTag = openFileTag(pathToRoot, fName, "./", "DeveloperNotes.html");
	document.write('Developer Notes' + closeTag + '</li>');

	document.write('</ul>');
}

function navigationTableTD(pathToRoot)
{
	document.write('<table align="left" style="padding-right:1em"><tr>');
	document.write('<td valign="top" bgcolor=' + navTableBGColor + '>');
	navigationTable(pathToRoot);
	document.write('</td></tr></table>');
}

function sectionHeader(header)
{
	document.write('<h2><font color=' + sectionHeaderColor + '>' + header + '</font></h2>');
}

function signAndDatePage(pathToRoot)
{
	document.write('<hr />');
	document.write('<img src="' + pathToRoot + 'images/ku_jayhawk.png" align="right" />');
	document.write('<p><font size="-2">James R. Miller (<a href="mailto:jrmiller@ku.edu">jrmiller@ku.edu</a>) &ndash;&nbsp;');
	document.write("Page last modified: " + document.lastModified + "</font></p>");
}
